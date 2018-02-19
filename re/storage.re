open Js;

open Schedule;

open BsReactNative;

module Schedule = {
  let key = "@myfosdem:schedule";
  let load = () =>
    Promise.(
      AsyncStorage.getItem(key, ())
      |> then_(json =>
           (
             switch json {
             | Some(json) =>
               try (Some(json |> Json.parseExn |> decode)) {
               | _ => None
               }
             | None => None
             }
           )
           |> resolve
         )
    );
  let save = schedule =>
    AsyncStorage.setItem(key, schedule |> encode |> Json.stringify, ());
};