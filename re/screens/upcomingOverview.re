open BsReactNative;

open Js;

let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([flex(1.), backgroundColor("white"), paddingTop(Pt(20.))]),
        "finished":
          style([
            fontSize(Float(18.)),
            paddingTop(Pt(10.)),
            paddingLeft(Pt(20.)),
            paddingRight(Pt(20.)),
            color("rgba(0, 0, 0, .8)")
          ]),
        "tweetContainer":
          style([
            flex(1.),
            alignItems(Center),
            justifyContent(Center),
            paddingTop(Pt(25.))
          ]),
        "tweetButton":
          style([
            paddingTop(Pt(10.)),
            paddingBottom(Pt(10.)),
            paddingLeft(Pt(35.)),
            paddingRight(Pt(35.)),
            marginTop(Pt(15.)),
            backgroundColor("#00aced"),
            borderRadius(50.)
          ]),
        "doneIcon":
          style([
            alignItems(Center),
            paddingTop(Pt(45.)),
            paddingBottom(Pt(45.))
          ])
      }
    )
  );

let tweet = () =>
  "https://twitter.com/intent/tweet?text=%23FOSDEM18"
  |> Linking.openURL
  |> ignore;

let alreadyFinished =
  <View>
    <View style=styles##doneIcon>
      <Icon name="check-circle" color="rgba(0,0,0,.07)" size=90 />
    </View>
    <Text
      style=styles##finished
      value="FOSDEM already finished and there are no upcoming talks."
    />
    <View style=styles##tweetContainer>
      <Text
        style=styles##finished
        value="Did you have a great time at FOSDEM this year?"
      />
      <View style=styles##tweetButton>
        <IconButton
          onPress=tweet
          title="Tweet about it!"
          accessibilityLabel="Tweet about your experience at FOSDEM this year"
          color="white"
          icon="twitter"
          iconSize=24
          fontSize=18
        />
      </View>
    </View>
  </View>;

let takeUpTo = (n, arr) => {
  let end_ = Array.length(arr) >= n ? n : Array.length(arr);
  Array.slice(arr, ~start=0, ~end_);
};

let upcomingEventsMaxNumber = 20;

let component = ReasonReact.statelessComponent("UpcomingOverview");

let make = (~schedule: Schedule.t, ~navigation, _) => {
  ...component,
  render: (_) => {
    let eventsByDay =
      Schedule.talksByDay(schedule)
      |> Array.slice(~start=0, ~end_=upcomingEventsMaxNumber)
      |> Schedule.upcomingTalks
      |> Schedule.groupByDay;
    <ScrollView style=styles##container>
      <Text value="Upcoming" style=Theme.styles##title />
      (
        if (Array.length(Dict.keys(eventsByDay)) == 0) {
          alreadyFinished;
        } else {
          ReasonReact.arrayToElement(
            Array.map(
              day =>
                <DayTalks
                  day
                  events=(Dict.unsafeGet(eventsByDay, day))
                  navigation
                  detailScreen="detail"
                  key=day
                />,
              Dict.keys(eventsByDay)
            )
          );
        }
      )
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