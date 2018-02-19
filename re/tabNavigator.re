/**
 * Reason bindings for ReactNative TabNavigator.
 * Slightly modified from https://github.com/FormidableLabs/seattlejsconf-app/blob/master/re/types/TabNavigator.re
 */
type iconOptions = {
  .
  "focused": Js.boolean,
  "tintColor": string
};

type dynamicTabBarIcon = iconOptions => ReasonReact.reactElement;

type routeConfig = {
  .
  "screen": ReasonReact.reactClass,
  "navigationOptions": {
    .
    "tabBarLabel": string,
    "tabBarIcon": dynamicTabBarIcon
  }
};

let route = (~screen, ~label, ~icon) => {
  "screen": screen,
  "navigationOptions": {
    "tabBarLabel": label,
    "tabBarIcon": icon
  }
};

type tabBarOptions = {
  .
  "activeTintColor": string,
  "inactiveTintColor": string,
  "activeBackgroundColor": string,
  "inactiveBackgroundColor": string
};

let options =
    (
      ~activeTintColor,
      ~inactiveTintColor,
      ~activeBackgroundColor,
      ~inactiveBackgroundColor
    ) => {
  "activeTintColor": activeTintColor,
  "inactiveTintColor": inactiveTintColor,
  "activeBackgroundColor": activeBackgroundColor,
  "inactiveBackgroundColor": inactiveBackgroundColor
};

type tabBarConfig = {
  .
  "animationEnabled": Js.boolean,
  "tabBarOptions": tabBarOptions,
  "tabBarPosition": string,
  "tabBarComponent": ReasonReact.reactClass
};

type tabBarPosition =
  | Top
  | Bottom;

[@bs.module "react-navigation"]
external top : ReasonReact.reactClass = "TabBarTop";

[@bs.module "react-navigation"]
external bottom : ReasonReact.reactClass = "TabBarBottom";

let config = (~animationEnabled, ~position, ~options) => {
  "animationEnabled": Js.Boolean.to_js_boolean(animationEnabled),
  "tabBarOptions": options,
  "tabBarPosition":
    switch position {
    | Top => "top"
    | Bottom => "bottom"
    },
  "tabBarComponent":
    switch position {
    | Top => top
    | Bottom => bottom
    }
};

[@bs.module "react-navigation"]
external tabNavigator :
  (Js.Dict.t(routeConfig), tabBarConfig) => ReasonReact.reactClass =
  "TabNavigator";

let make = (routes, config) => tabNavigator(Js.Dict.fromList(routes), config);