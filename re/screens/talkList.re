open BsReactNative;

let arrayGet = Array.get; /* after opening Js Array.get is gone */

open Js;

type state = {
  selectedIndex: int,
  search: string,
  talksByDay: Dict.t(array(Schedule.Event.t))
};

type action =
  | SelectDay(int)
  | ChangeSearch(string);

let component = ReasonReact.reducerComponent("TalkList");

let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([flex(1.), backgroundColor("white"), paddingTop(Pt(20.))]),
        "title":
          style([
            fontSize(Float(35.)),
            fontWeight(`Bold),
            paddingTop(Pt(20.)),
            paddingLeft(Pt(15.)),
            paddingBottom(Pt(15.))
          ]),
        "header":
          style([
            borderBottomWidth(1.),
            borderBottomColor(Theme.lightBorderColor)
          ]),
        "dayPicker": style([paddingLeft(Pt(15.)), paddingRight(Pt(15.))])
      }
    )
  );

let toList = arr =>
  arr
  |> Array.reverseInPlace
  |> Array.reduce((ls, elem) => List.cons(elem, ls), []);

[@bs.get] external selectedSegmentIndex : RNEvent.NativeEvent.t => int = "";

let make = (~schedule: Schedule.t, ~navigation, _) => {
  ...component,
  initialState: () => {
    selectedIndex: 0,
    search: "",
    talksByDay: Schedule.talksByDay(schedule) |> Schedule.groupByDay
  },
  reducer: (action, state) =>
    switch action {
    | SelectDay(selectedIndex) => ReasonReact.Update({...state, selectedIndex})
    | ChangeSearch(search) => ReasonReact.Update({...state, search})
    },
  render: self => {
    let keys = Dict.keys(self.state.talksByDay);
    let values = keys |> Array.map(day => Dates.dayName(day)) |> toList;
    let selectedDay = arrayGet(keys, self.state.selectedIndex);
    let dayEvents = Dict.unsafeGet(self.state.talksByDay, selectedDay);
    let events =
      if (self.state.search == "") {
        dayEvents;
      } else {
        let search = String.toLowerCase(self.state.search);
        Array.filter(
          (e: Schedule.Event.t) =>
            String.indexOf(search, String.toLowerCase(e.title)) >= 0,
          dayEvents
        );
      };
    <View style=styles##container>
      <View style=styles##header>
        <Text value="Talks" style=styles##title />
        <View style=styles##dayPicker>
          <SegmentedControllOS
            values
            selectedIndex=self.state.selectedIndex
            tintColor=Theme.accent
            onChange=(
              (e: RNEvent.t) =>
                self.send(
                  SelectDay(e |> RNEvent.nativeEvent |> selectedSegmentIndex)
                )
            )
          />
        </View>
        <SearchBar
          query=self.state.search
          onChangeText=(text => self.send(ChangeSearch(text)))
        />
      </View>
      <ScrollView style=styles##container>
        <DayTalks
          day=selectedDay
          key=selectedDay
          events
          detailScreen="talks-talk-detail"
          navigation
        />
      </ScrollView>
    </View>;
  }
};

let jsComponent =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(
      ~schedule=jsProps##screenProps##schedule,
      ~navigation=StackNavigator.navigation(jsProps),
      [||]
    )
  );
