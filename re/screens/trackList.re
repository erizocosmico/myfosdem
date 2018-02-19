open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([flex(1.), backgroundColor("white"), paddingTop(Pt(20.))]),
        "trackTitle": style([fontSize(Float(18.)), fontWeight(`Bold)]),
        "trackTalks": style([color(Theme.textLightColor), paddingTop(Pt(3.))])
      }
    )
  );

let component = ReasonReact.statelessComponent("TrackList");

let keyFromTrack = ((track, _), _) => track;

let goToTrackTalks = (navigation, schedule, track: string, ()) =>
  StackNavigator.navigate(
    navigation,
    "track-talks",
    {
      "title": track,
      "detailScreen": "track-talk-detail",
      "talks": Schedule.talksForTrack(schedule, ~track)
    }
  );

let renderTrack =
    (navigation, schedule, {item}: FlatList.renderBag((string, int))) => {
  let (track, talks) = item;
  <TouchableOpacity
    onPress=(goToTrackTalks(navigation, schedule, track))
    style=Theme.styles##row>
    <Text value=track style=styles##trackTitle />
    <Text value=(string_of_int(talks) ++ " talks") style=styles##trackTalks />
  </TouchableOpacity>;
};

let make = (~schedule: Schedule.t, ~navigation, _) => {
  ...component,
  render: (_) => {
    let tracks = Schedule.tracksWithTalkCount(schedule);
    <ScrollView style=styles##container>
      <Text value="Tracks" style=Theme.styles##title />
      <FlatList
        data=tracks
        keyExtractor=keyFromTrack
        renderItem=(FlatList.renderItem(renderTrack(navigation, schedule)))
      />
    </ScrollView>;
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
