[@bs.module "react-native-vector-icons/Feather"]
external feather : ReasonReact.reactClass = "default";

[@bs.module "react-native-vector-icons/Entypo"]
external entypo : ReasonReact.reactClass = "default";

let make = (~kind=feather, ~name: string, ~size: int, ~color: string, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=kind,
    ~props={"name": name, "size": size, "color": color},
    children
  );