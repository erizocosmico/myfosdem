open Js;

let dayName = date =>
  switch (Date.fromString(date) |> Date.getDay) {
  | 0. => "Sunday"
  | 1. => "Monday"
  | 2. => "Tuesday"
  | 3. => "Wednesday"
  | 4. => "Thursday"
  | 5. => "Friday"
  | 6. => "Saturday"
  | _ => ""
  };

let pretty = date => {
  let d = Date.fromString(date);
  let day = Date.getDate(d);
  let month =
    switch (Date.getMonth(d)) {
    | 0. => "January"
    | 1. => "February"
    | 2. => "March"
    | 3. => "April"
    | 4. => "May"
    | 5. => "June"
    | 6. => "July"
    | 7. => "August"
    | 8. => "September"
    | 9. => "October"
    | 10. => "November"
    | 11. => "December"
    | _ => ""
    };
  string_of_int(int_of_float(day)) ++ " " ++ month;
};