[@bs.module "recyclerlistview"]
external view : ReasonReact.reactClass = "RecyclerListView";

type dim = {
  .
  "height": int,
  "width": int
};

[@bs.set] external setHeight : (dim, int) => unit = "height";

[@bs.set] external setWidth : (dim, int) => unit = "width";

type layoutProvider;

[@bs.new] [@bs.module "recyclerlistview"]
external makeLayoutProvider : (int => int, (int, dim) => dim) => layoutProvider =
  "LayoutProvider";

type dataProvider;

[@bs.new] [@bs.module "recyclerlistview"]
external makeDataProvider : (('a, 'a) => bool) => dataProvider =
  "DataProvider";

[@bs.send.pipe : dataProvider]
external cloneWithRows : array('a) => dataProvider = "";

let make =
    (
      ~rowRenderer: (int, 'item) => ReasonReact.reactElement,
      ~layoutProvider: layoutProvider,
      ~dataProvider: dataProvider,
      ~style: 'a
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=view,
    ~props={
      "rowRenderer": rowRenderer,
      "layoutProvider": layoutProvider,
      "dataProvider": dataProvider,
      "canChangeSize": Js.true_,
      "forceNonDeterministicRendering": Js.true_,
      "style": style
    }
  );