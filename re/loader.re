open BsReactNative;

open Js;

type action =
  | LoadedSchedule(Schedule.t)
  | MarkFavorite(int)
  | RemoveFavorite(int);

type state =
  | Loading
  | Loaded(Schedule.t)
  | Error;

let component = ReasonReact.reducerComponent("Loader");

let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([
            flex(1.),
            justifyContent(Center),
            alignItems(Center),
            backgroundColor("white")
          ]),
        "loading": style([fontSize(Float(18.)), fontWeight(`Bold)]),
        "errorMsg":
          style([fontSize(Float(18.)), fontWeight(`Bold), color("#db4d3f")])
      }
    )
  );

let make = () => {
  ...component,
  initialState: () => Loading,
  didMount: (_) =>
    ReasonReact.SideEffects(
      self =>
        Storage.Schedule.load()
        |> Promise.then_(schedule =>
             switch schedule {
             | Some(schedule) =>
               self.ReasonReact.send(LoadedSchedule(schedule))
               |> Promise.resolve
             | None =>
               Schedule.get()
               |> Promise.then_(schedule =>
                    Storage.Schedule.save(schedule)
                    |> Promise.then_(() =>
                         self.ReasonReact.send(LoadedSchedule(schedule))
                         |> Promise.resolve
                       )
                  )
             /*** TODO: handle exception */
             }
           )
        |> ignore
    ),
  /*** TODO: update in storage every time a fav is changed */
  reducer: (action, state) =>
    switch (action, state) {
    | (LoadedSchedule(schedule), _) => ReasonReact.Update(Loaded(schedule))
    | (MarkFavorite(id), Loaded(schedule)) =>
      ReasonReact.Update(
        Loaded(
          Array.map(
            (e: Schedule.Event.t) => e.id == id ? {...e, favorite: true} : e,
            schedule
          )
        )
      )
    | (RemoveFavorite(id), Loaded(schedule)) =>
      ReasonReact.Update(
        Loaded(
          Array.map(
            (e: Schedule.Event.t) => e.id == id ? {...e, favorite: false} : e,
            schedule
          )
        )
      )
    | _ => ReasonReact.NoUpdate
    },
  render: self =>
    switch self.state {
    | Loading =>
      <View style=styles##container>
        <Text value="Loading schedule..." style=styles##loading />
      </View>
    | Loaded(schedule) => <Root schedule />
    | Error =>
      <View style=styles##container>
        <Text value="Unable to load FOSDEM schedule" style=styles##errorMsg />
      </View>
    }
};

let jsComponent = ReasonReact.wrapReasonForJs(~component, (_) => make());
