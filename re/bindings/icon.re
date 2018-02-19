[@bs.module "@expo/vector-icons"]
external feather : ReasonReact.reactClass = "Feather";

[@bs.module "@expo/vector-icons"]
external entypo : ReasonReact.reactClass = "Entypo";

let make = (~kind=feather, ~name: string, ~size: int, ~color: string, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=kind,
    ~props={"name": name, "size": size, "color": color},
    children
  );
