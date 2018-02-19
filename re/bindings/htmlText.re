[@bs.module "react-native-htmlview"]
external htmlView : ReasonReact.reactClass = "default";

let make = (~value, ~styles, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=htmlView,
    ~props={"value": value, "stylesheet": styles, "addLineBreaks": Js.false_},
    children
  );