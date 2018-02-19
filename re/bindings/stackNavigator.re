open Js;

type navigationOptions;

type navigationProps('a) = {
  .
  "navigation": {
    .
    "state": {
      .
      "key": string,
      "routeName": string,
      "params": Js.Undefined.t(Js.t('a))
    }
  }
};

external navigationOptions :
  (navigationProps('a) => Js.t('b)) => navigationOptions =
  "%identity";

type routeConfig = {
  .
  "screen": ReasonReact.reactClass,
  "navigationOptions": navigationOptions
};

type headerTitle('a) =
  | Static(string)
  | Dynamic('a => string)
  | Component('a => ReasonReact.reactElement);

type any;

external any : 'a => any = "%identity";

let route =
    (
      ~header=Nullable.undefined,
      ~headerTitle=Static(""),
      ~headerTintColor="",
      screen
    ) => {
  "screen": screen,
  "navigationOptions":
    navigationOptions(jsOpts => {
      let params =
        switch (Undefined.to_opt(jsOpts##navigation##state##params)) {
        | Some(params) => params
        | None => Obj.empty()
        };
      let title =
        switch headerTitle {
        | Static(t) => any(t)
        | Dynamic(callback) => any(callback(params))
        | Component(callback) => any(callback(params))
        };
      {
        "header": header,
        "headerTitle": title,
        "headerTintColor": headerTintColor
      };
    })
};

type mode =
  | Card
  | Modal;

type headerMode =
  | Float
  | Screen
  | NoHeader;

type stackNavigationConfig = {
  .
  "initialRouteName": string,
  "mode": string,
  "headerMode": string
};

let config = (~mode=Card, ~headerMode=Float, initialRouteName) => {
  "initialRouteName": initialRouteName,
  "mode":
    switch mode {
    | Card => "card"
    | Modal => "modal"
    },
  "headerMode":
    switch headerMode {
    | Float => "float"
    | Screen => "screen"
    | NoHeader => "none"
    }
};

[@bs.module "react-navigation"]
external stackNavigator :
  (Js.Dict.t(routeConfig), stackNavigationConfig) => ReasonReact.reactClass =
  "StackNavigator";

let make = (routes, config) =>
  stackNavigator(Js.Dict.fromList(routes), config);

type navigation;

[@bs.send] external navigate : (navigation, string, Js.t('a)) => unit = "";

external toNavigation : Js.t('a) => navigation = "%identity";

let navigation = jsProps => toNavigation(jsProps##navigation);

[@bs.send] external goBack : navigation => unit = "";
