[@bs.module "@expo/vector-icons"]
external icons : ReasonReact.reactClass = "Feather";

let make = (~name: string, ~size: int, ~color: string, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=icons,
    ~props={"name": name, "size": size, "color": color},
    children
  );