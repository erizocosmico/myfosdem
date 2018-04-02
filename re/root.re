let icon = (name, config: TabNavigator.iconOptions) =>
  <Icon name size=24 color=config##tintColor />;

let config =
  TabNavigator.(
    config(
      ~animationEnabled=true,
      ~position=Bottom,
      ~isLazy=true,
      ~options=
        options(
          ~activeTintColor=Theme.accent,
          ~inactiveTintColor=Theme.tabBarTintColor,
          ~activeBackgroundColor="white",
          ~inactiveBackgroundColor="white"
        )
    )
  );

let routes = [
  (
    "overview",
    TabNavigator.route(
      ~screen=Upcoming.component,
      ~label="Upcoming",
      ~icon=icon("clock")
    )
  ),
  (
    "talks",
    TabNavigator.route(
      ~screen=Talks.component,
      ~label="Talks",
      ~icon=icon("message-circle")
    )
  ),
  (
    "tracks",
    TabNavigator.route(
      ~screen=Tracks.component,
      ~label="Tracks",
      ~icon=icon("list")
    )
  ),
  (
    "favorites",
    TabNavigator.route(
      ~screen=Favorites.component,
      ~label="Favorites",
      ~icon=icon("star")
    )
  )
];

let component = TabNavigator.make(routes, config);

let make = (~schedule: Schedule.t, ~favorite, ~removeFavorite, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=component,
    ~props={
      "screenProps": {
        "schedule": schedule,
        "favorite": favorite,
        "removeFavorite": removeFavorite
      }
    },
    children
  );