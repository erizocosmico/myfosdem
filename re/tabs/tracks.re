open Js;

let routes = [
  (
    "tracks",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Static("Tracks"),
        TrackList.jsComponent
      )
    )
  ),
  (
    "track-talks",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Dynamic(jsProps => jsProps##title),
        TrackTalkList.jsComponent
      )
    )
  ),
  (
    "track-talk-detail",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Dynamic(jsProps => jsProps##title),
        Talk.jsComponent
      )
    )
  )
];

let component =
  StackNavigator.make(
    routes,
    StackNavigator.(config("tracks", ~headerMode=Screen))
  );
