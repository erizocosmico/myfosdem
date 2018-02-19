open BsReactNative;

let component = ReasonReact.statelessComponent("EventList");

let styles =
  StyleSheet.create(
    Style.(
      {
        "container": style([flex(1.)]),
        "title": style([fontSize(Float(18.)), fontWeight(`Bold)]),
        "subtitle": style([color(Theme.textMediumColor), paddingTop(Pt(3.))]),
        "time": style([color(Theme.textLightColor), fontWeight(`Bold)]),
        "track":
          style([
            fontSize(Float(12.)),
            color(Theme.textLightColor),
            paddingLeft(Pt(10.))
          ]),
        "info":
          style([
            flex(1.),
            flexDirection(Row),
            alignItems(Center),
            paddingTop(Pt(5.))
          ])
      }
    )
  );

let keyFromEvent = (event: Schedule.Event.t, _) => string_of_int(event.id);

let goToDetail = (navigation, detailScreen, item: Schedule.Event.t, ()) =>
  StackNavigator.navigate(
    navigation,
    detailScreen,
    {"talk": string_of_int(item.id), "title": item.title}
  );

let renderEvent =
    (
      ~showRoom,
      ~showTrack,
      ~showDate,
      ~navigation,
      ~detailScreen,
      {item}: VirtualizedList.renderBag(Schedule.Event.t)
    ) =>
  <TouchableOpacity
    onPress=(goToDetail(navigation, detailScreen, item))
    style=Theme.styles##row>
    <Text value=item.title style=styles##title />
    (
      item.subtitle == "" ?
        ReasonReact.nullElement :
        <Text value=item.subtitle style=styles##subtitle />
    )
    <View style=styles##info>
      <Text value=(Schedule.Time.toString(item.start)) style=styles##time />
      (
        showTrack ?
          <Text value=item.track style=styles##track /> :
          ReasonReact.nullElement
      )
      (
        showDate ?
          <Text value=(Dates.dayName(item.date)) style=styles##track /> :
          ReasonReact.nullElement
      )
      (
        showRoom ?
          <Text value=("at " ++ item.room) style=styles##track /> :
          ReasonReact.nullElement
      )
    </View>
  </TouchableOpacity>;

let make =
    (
      ~events: array(Schedule.Event.t),
      ~detailScreen="detail",
      ~navigation,
      ~showTrack=true,
      ~showDate=false,
      ~showRoom=false,
      _
    ) => {
  ...component,
  render: (_) =>
    <VirtualizedList
      data=events
      keyExtractor=keyFromEvent
      initialNumToRender=10
      windowSize=30
      maxToRenderPerBatch=30
      getItemCount=(data => Array.length(data))
      getItem=((data, i) => data[i])
      renderItem=(
        VirtualizedList.renderItem(
          renderEvent(
            ~navigation,
            ~detailScreen,
            ~showRoom,
            ~showTrack,
            ~showDate
          )
        )
      )
    />
};
