type content = {
  .
  "message": string,
  "title": string,
  "url": string
};

let content = (~title="", ~url="", ~message: string, ()) => {
  "title": title,
  "message": message,
  "url": url
};

type options = {
  .
  "subject": string,
  "excludedActivityTypes": array(string),
  "tintColor": string
};

let options = (~subject="", ~excludedActivityTypes=[||], ~tintColor="", ()) => {
  "subject": subject,
  "excludedActivityTypes": excludedActivityTypes,
  "tintColor": tintColor
};

type t;

[@bs.module "react-native"] external sharer : t = "Share";

[@bs.send]
external doShare : (t, content, options) => Js.Promise.t(unit) = "share";

let share = (content, options) =>
  doShare(sharer, content, options)
  |> Js.Promise.then_(() => Js.Promise.resolve())
  |> ignore;
