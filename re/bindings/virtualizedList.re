[@bs.module "react-native"]
external view : ReasonReact.reactClass = "VirtualizedList";

type jsRenderBag('item) = {
  .
  "item": 'item,
  "index": int
};

type renderBag('item) = {
  item: 'item,
  index: int
};

type renderItem('item) = jsRenderBag('item) => ReasonReact.reactElement;

let renderItem =
    (reRenderItem: renderBag('item) => ReasonReact.reactElement)
    : renderItem('item) =>
  (jsRenderBag: jsRenderBag('item)) =>
    reRenderItem({item: jsRenderBag##item, index: jsRenderBag##index});

let make =
    (
      ~data: array('item),
      ~renderItem: renderItem('item),
      ~getItem: (array('item), int) => 'item,
      ~getItemCount: array('item) => int,
      ~keyExtractor: ('item, int) => string,
      ~initialNumToRender=?,
      ~windowSize=?,
      ~maxToRenderPerBatch=?
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=view,
    ~props=
      Js.Undefined.(
        {
          "data": data,
          "getItem": getItem,
          "getItemCount": getItemCount,
          "initialNumToRender": from_opt(initialNumToRender),
          "keyExtractor": keyExtractor,
          "renderItem": renderItem,
          "windowSize": from_opt(windowSize),
          "maxToRenderPerBatch": from_opt(maxToRenderPerBatch)
        }
      )
  );
