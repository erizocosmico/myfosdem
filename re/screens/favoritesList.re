open BsReactNative;

type state =
  | Loading
  | Loaded(array(Schedule.Event.t));

type action =
  | LoadedFavs(array(int));

let component = ReasonReact.statelessComponent("FavoritesList");

let styles =
  StyleSheet.create(
    Style.(
      {
        "noFavsContainer": style([alignItems(Center), marginTop(Pt(150.))]),
        "noFavsTitle":
          style([
            fontSize(Float(25.)),
            paddingTop(Pt(15.)),
            paddingBottom(Pt(5.)),
            color(Theme.textLightColor)
          ]),
        "noFavsSubtitle":
          style([fontSize(Float(13.)), color(Theme.textLighterColor)])
      }
    )
  );

let make = (~schedule: Schedule.t, ~navigation, _) => {
  ...component,
  render: (_) =>
    switch (Schedule.favorites(schedule)) {
    | [||] =>
      <View style=Theme.styles##container>
        <View style=Theme.styles##statusBar />
        <Text value="Favorites" style=Theme.styles##title />
        <View style=styles##noFavsContainer>
          <Icon name="star" color="rgba(0,0,0,.07)" size=60 />
          <Text value="No favorite talks yet" style=styles##noFavsTitle />
          <Text
            value="Go favorite some awesome talks!"
            style=styles##noFavsSubtitle
          />
        </View>
      </View>
    | talks =>
      <View style=Theme.styles##container>
        <View style=Theme.styles##statusBar />
        <ScrollView style=Theme.styles##scrollview>
          <Text value="Favorites" style=Theme.styles##title />
          <EventList
            events=talks
            showDate=true
            showRoom=true
            showTrack=true
            detailScreen="favorites-talk-detail"
            navigation
          />
        </ScrollView>
      </View>
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
