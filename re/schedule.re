let firstElementText = (node, element) =>
  Js.Option.map(
    [@bs] (node => Xml.text(node)),
    Xml.elements(node, element) |> Xml.first
  )
  |> Js.Option.getWithDefault("");

let optToInt = opt => Js.Option.map([@bs] (v => int_of_string(v)), opt);

module Person = {
  type t = {
    id: int,
    name: string
  };
  let ofXml = node => {
    id: Xml.attribute(node, "id") |> optToInt |> Js.Option.getWithDefault(0),
    name: Xml.text(node)
  };
  let encode = person =>
    Json.Encode.(
      object_([("id", person.id |> int), ("name", person.name |> string)])
    );
  let decode = json =>
    Json.Decode.{
      id: json |> field("id", int),
      name: json |> field("name", string)
    };
};

module Link = {
  type t = {
    title: string,
    url: string
  };
  let ofXml = node => {
    title: Xml.text(node),
    url: Xml.attribute(node, "href") |> Js.Option.getWithDefault("")
  };
  let encode = link =>
    Json.Encode.(
      object_([("title", link.title |> string), ("url", link.url |> string)])
    );
  let decode = json =>
    Json.Decode.{
      title: json |> field("title", string),
      url: json |> field("url", string)
    };
};

module Time = {
  type t = {
    hours: int,
    minutes: int
  };
  let ofString = str =>
    switch (Js.String.split(":", str)) {
    | [|h, m|] => {hours: int_of_string(h), minutes: int_of_string(m)}
    | _ => {hours: 0, minutes: 0}
    };
  let encode = ({hours, minutes}) =>
    string_of_int(hours) ++ ":" ++ string_of_int(minutes);
  let compare = (a, b) =>
    switch (a.hours - b.hours) {
    | 0 => a.minutes - b.minutes
    | n => n
    };
  let timeComponentString = n =>
    n < 10 ? "0" ++ string_of_int(n) : string_of_int(n);
  let toString = ({hours, minutes}) =>
    timeComponentString(hours) ++ ":" ++ timeComponentString(minutes);
  let toMillis = ({hours, minutes}) =>
    float_of_int((hours * 3600 + minutes * 60) * 1000);
};

module Event = {
  type t = {
    id: int,
    start: Time.t,
    date: string,
    duration: int,
    room: string,
    title: string,
    subtitle: string,
    slug: string,
    track: string,
    abstract: string,
    description: string,
    people: array(Person.t),
    links: array(Link.t),
    favorite: bool
  };
  let ofXml = (~date="", node) => {
    id: Xml.attribute(node, "id") |> optToInt |> Js.Option.getWithDefault(0),
    start: firstElementText(node, "start") |> Time.ofString,
    duration:
      Js.Option.map(
        [@bs]
        (
          node =>
            switch (Js.String.split(":", Xml.text(node))) {
            | [|h, m|] => int_of_string(h) * 60 + int_of_string(m)
            | _ => 0
            }
        ),
        Xml.elements(node, "duration") |> Xml.first
      )
      |> Js.Option.getWithDefault(0),
    date,
    room: firstElementText(node, "room"),
    title: firstElementText(node, "title"),
    subtitle: firstElementText(node, "subtitle"),
    slug: firstElementText(node, "slug"),
    track: firstElementText(node, "track"),
    abstract: firstElementText(node, "abstract"),
    description: firstElementText(node, "description"),
    people: Xml.elements(node, "person") |> Array.map(Person.ofXml),
    links: Xml.elements(node, "link") |> Array.map(Link.ofXml),
    favorite: false
  };
  let ofRoomXml = (~date, node) : array(t) =>
    Xml.elements(node, "event") |> Array.map(ofXml(~date));
  let ofDayXml = node : array(t) => {
    open Js;
    let date = Xml.attribute(node, "date") |> Js.Option.getWithDefault("");
    Xml.elements(node, "room")
    |> Array.map(ofRoomXml(~date))
    |> Array.reduce(Js.Array.concat, [||]);
  };
  let encode = event =>
    Json.Encode.(
      object_([
        ("id", event.id |> int),
        ("start", event.start |> Time.encode |> string),
        ("duration", event.duration |> int),
        ("room", event.room |> string),
        ("date", event.date |> string),
        ("title", event.title |> string),
        ("subtitle", event.subtitle |> string),
        ("slug", event.slug |> string),
        ("track", event.track |> string),
        ("abstract", event.abstract |> string),
        ("description", event.description |> string),
        ("people", event.people |> array(Person.encode)),
        ("links", event.links |> array(Link.encode)),
        ("favorite", event.favorite |> bool)
      ])
    );
  let decode = json =>
    Json.Decode.{
      id: json |> field("id", int),
      start: json |> field("start", string) |> Time.ofString,
      duration: json |> field("duration", int),
      room: json |> field("room", string),
      date: json |> field("date", string),
      title: json |> field("title", string),
      subtitle: json |> field("subtitle", string),
      slug: json |> field("slug", string),
      track: json |> field("track", string),
      abstract: json |> field("abstract", string),
      description: json |> field("description", string),
      people: json |> field("people", array(Person.decode)),
      links: json |> field("links", array(Link.decode)),
      favorite: json |> field("favorite", bool)
    };
};

type t = array(Event.t);

let ofXml = node =>
  Js.(
    Xml.elements(node, "day")
    |> Array.map(Event.ofDayXml)
    |> Array.reduce(Js.Array.concat, [||])
  );

let encode = schedule => schedule |> Json.Encode.array(Event.encode);

let decode = json => json |> Json.Decode.array(Event.decode);

let url = "https://fosdem.org/2018/schedule/xml";

let get = () =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.text)
    |> then_(text => Xml.parse(text) |> ofXml |> resolve)
  );

let findTalk = (schedule: t, id: int) =>
  Js.(
    Array.filter((e: Event.t) => e.id == id, schedule)
    |> Array.shift
    |> Option.getExn
  );

let tracksWithTalkCount = (schedule: t) : array((string, int)) => {
  open Js;
  let tracks = Dict.empty();
  Array.forEach(
    (event: Event.t) => {
      let talks =
        switch (Dict.get(tracks, event.track)) {
        | Some(talks) => talks + 1
        | None => 1
        };
      Dict.set(tracks, event.track, talks);
    },
    schedule
  );
  Dict.entries(tracks);
};

let talksForTrack = (~track: string, schedule: t) =>
  Js.Array.filter((e: Event.t) => e.track == track, schedule);

let groupByDay = events : Js.Dict.t(array(Event.t)) =>
  Js.(
    if (Array.length(events) == 0) {
      Dict.empty();
    } else {
      let groups: Dict.t(array(Event.t)) = Dict.empty();
      Array.forEach(
        ((d, e: Event.t)) =>
          switch (Dict.get(groups, d)) {
          | Some(arr) => Array.push(e, arr) |> ignore
          | None => Dict.set(groups, d, [|e|])
          },
        events
      );
      groups;
    }
  );

let talksByDay = (schedule: t) =>
  Js.(
    Array.map((e: Event.t) => (e.date, e), schedule)
    |> Array.sortInPlaceWith(((d1, e1: Event.t), (d2, e2: Event.t)) => {
         let t1 = d1 |> Date.fromString |> Date.getTime;
         let t2 = d2 |> Date.fromString |> Date.getTime;
         switch (int_of_float(t1 -. t2)) {
         | 0 => Time.compare(e1.start, e2.start)
         | n => n
         };
       })
  );

let talksByIds = (~ids: array(int), schedule: t) => {
  open Js;
  let ids = Array.map(n => (string_of_int(n), true), ids) |> Dict.fromArray;
  Array.filter(
    (event: Event.t) =>
      Dict.get(ids, string_of_int(event.id)) |> Option.isSome,
    schedule
  );
};

let upcomingTalks = events => {
  open Js;
  let now = Date.now();
  Array.filter(
    ((_, e: Event.t)) => {
      let then_ = Date.fromString(e.date) |> Date.getTime;
      let t = then_ +. Time.toMillis(e.start);
      t -. now > 0.;
    },
    events
  );
};

let favorites = (schedule: t) =>
  Js.Array.filter((e: Event.t) => e.favorite, schedule);
