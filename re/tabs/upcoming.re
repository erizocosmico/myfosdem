open Js;

let routes = [
  (
    "overview",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Static("Upcoming"),
        ~headerTintColor="#ab5ea3",
        UpcomingOverview.jsComponent
      )
    )
  ),
  (
    "detail",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Dynamic(jsProps => jsProps##title),
        ~headerTintColor="#ab5ea3",
        Talk.jsComponent
      )
    )
  )
];

let component =
  StackNavigator.make(
    routes,
    StackNavigator.(config("overview", ~headerMode=Screen))
  );
