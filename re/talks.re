open Js;

let routes = [
  (
    "talk-list",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Static("Talks"),
        TalkList.jsComponent
      )
    )
  ),
  (
    "talks-talk-detail",
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
    StackNavigator.(config("talk-list", ~headerMode=Screen))
  );