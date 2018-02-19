open BsReactNative;

let arrayGet = Array.get; /* after opening Js Array.get is gone */

open Js;

type state = {
  selectedIndex: int,
  talksByDay: Dict.t(array(Schedule.Event.t))
};

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
    talksByDay: Schedule.talksByDay(schedule) |> Schedule.groupByDay
  },
  reducer: (newIndex, state) =>
    ReasonReact.Update({...state, selectedIndex: newIndex}),
  render: self => {
    let keys = Dict.keys(self.state.talksByDay);
    let values = keys |> Array.map(day => Dates.dayName(day)) |> toList;
    let selectedDay = arrayGet(keys, self.state.selectedIndex);
    let events = Dict.unsafeGet(self.state.talksByDay, selectedDay);
    <ScrollView style=styles##container>
      <Text value="Talks" style=styles##title />
      <View style=styles##dayPicker>
        <SegmentedControllOS
          values
          selectedIndex=self.state.selectedIndex
          tintColor=Theme.accent
          onChange=(
            (e: RNEvent.t) =>
              self.send(e |> RNEvent.nativeEvent |> selectedSegmentIndex)
          )
        />
      </View>
      <DayTalks
        day=selectedDay
        key=selectedDay
        events
        detailScreen="talks-talk-detail"
        navigation
      />
    </ScrollView>;
  }
};

let jsComponent =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(
      ~schedule=jsProps##screenProps,
      ~navigation=StackNavigator.navigation(jsProps),
      [||]
    )
  );