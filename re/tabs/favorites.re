open Js;

let routes = [
  (
    "favorites",
    StackNavigator.(
      route(
        ~header=Nullable.null,
        ~headerTitle=Static("Favorites"),
        FavoritesList.jsComponent
      )
    )
  ),
  (
    "favorites-talk-detail",
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
    StackNavigator.(config("favorites", ~headerMode=Screen))
  );
