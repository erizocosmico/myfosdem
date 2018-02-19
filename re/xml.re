module Xml = {
  type parser;
  type node = {. "textContent": string};
  type nodeList;
  [@bs.new] [@bs.module "xmldom"]
  external makeParser : unit => parser = "DOMParser";
  [@bs.send] external parse : (parser, string) => node = "parseFromString";
  [@bs.send]
  external elements : (node, string) => nodeList = "getElementsByTagName";
  [@bs.val] external toArray : nodeList => array(node) = "Array.from";
  [@bs.send]
  external attribute : (node, string) => Js.nullable(string) = "getAttribute";
};

let parse = (content: string) => Xml.parse(Xml.makeParser(), content);

let elements = (node, tagName) => Xml.elements(node, tagName) |> Xml.toArray;

let first = nodes =>
  if (Array.length(nodes) > 0) {
    Some(nodes[0]);
  } else {
    None;
  };

let text = node => node##textContent;

let attribute = (node, attr) => Xml.attribute(node, attr) |> Js.toOption;
