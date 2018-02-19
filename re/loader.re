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

let updateAndSaveSchedule = schedule =>
  ReasonReact.UpdateWithSideEffects(
    Loaded(schedule),
    (_) =>
      Storage.Schedule.save(schedule)
      |> Promise.then_(Promise.resolve)
      |> ignore
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
                    self.ReasonReact.send(LoadedSchedule(schedule))
                    |> Promise.resolve
                  )
             /*** TODO: handle exception */
             }
           )
        |> ignore
    ),
  reducer: (action, state) =>
    switch (action, state) {
    | (LoadedSchedule(schedule), _) => updateAndSaveSchedule(schedule)
    | (MarkFavorite(id), Loaded(schedule)) =>
      updateAndSaveSchedule(
        Array.map(
          (e: Schedule.Event.t) => e.id == id ? {...e, favorite: true} : e,
          schedule
        )
      )
    | (RemoveFavorite(id), Loaded(schedule)) =>
      updateAndSaveSchedule(
        Array.map(
          (e: Schedule.Event.t) => e.id == id ? {...e, favorite: false} : e,
          schedule
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
    | Loaded(schedule) =>
      <Root
        schedule
        favorite=(id => self.send(MarkFavorite(id)))
        removeFavorite=(id => self.send(RemoveFavorite(id)))
      />
    | Error =>
      <View style=styles##container>
        <Text value="Unable to load FOSDEM schedule" style=styles##errorMsg />
      </View>
    }
};

let jsComponent = ReasonReact.wrapReasonForJs(~component, (_) => make());
