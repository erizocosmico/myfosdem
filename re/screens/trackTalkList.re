open BsReactNative;

let component = ReasonReact.statelessComponent("TrackTalkList");

let styles =
  StyleSheet.create(
    Style.(
      {
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

let make = (~title, ~talks, ~detailScreen, ~navigation, _) => {
  ...component,
  render: (_) =>
    <View style=Theme.styles##container>
      <View style=Theme.styles##nav>
        <IconButton
          title="Back"
          icon="arrow-left"
          onPress=(() => StackNavigator.goBack(navigation))
          fontSize=16
          iconSize=25
          color=Theme.accent
        />
      </View>
      <ScrollView style=Theme.styles##scrollview>
        <Text value=title style=styles##title />
        <EventList
          events=talks
          showDate=true
          showRoom=true
          showTrack=false
          detailScreen
          navigation
        />
      </ScrollView>
    </View>
};

let jsComponent =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(
      ~navigation=StackNavigator.navigation(jsProps),
      ~title=jsProps##navigation##state##params##title,
      ~talks=jsProps##navigation##state##params##talks,
      ~detailScreen=jsProps##navigation##state##params##detailScreen,
      [||]
    )
  );
