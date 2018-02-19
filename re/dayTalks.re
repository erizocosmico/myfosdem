open BsReactNative;

open Js;

let component = ReasonReact.statelessComponent("DayTalks");

let styles =
  StyleSheet.create(
    Style.(
      {
        "container": style([flex(1.), paddingBottom(Pt(20.))]),
        "title":
          style([
            fontSize(Float(28.)),
            fontWeight(`Bold),
            paddingTop(Pt(0.)),
            paddingLeft(Pt(15.)),
            paddingBottom(Pt(15.))
          ]),
        "subtitle":
          style([
            fontWeight(`Bold),
            fontSize(Float(12.)),
            paddingLeft(Pt(15.)),
            paddingTop(Pt(20.)),
            color("rgba(0, 0, 0, .4)")
          ])
      }
    )
  );

let make =
    (
      ~detailScreen: string,
      ~day: string,
      ~events: array(Schedule.Event.t),
      ~navigation,
      _
    ) => {
  ...component,
  render: (_) => {
    let dayName = Dates.dayName(day);
    let date = Dates.pretty(day) |> String.toUpperCase;
    <View style=styles##container>
      <Text value=date style=styles##subtitle />
      <Text value=dayName style=styles##title />
      <EventList events detailScreen navigation />
    </View>;
  }
};