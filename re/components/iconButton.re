open BsReactNative;

let component = ReasonReact.statelessComponent("IconButton");

let make =
    (
      ~onPress=() => (),
      ~color="",
      ~disabled=false,
      ~accessibilityLabel="",
      ~title: string,
      ~icon: string,
      ~iconSize: int,
      ~fontSize: int,
      ~bold=false,
      _
    ) => {
  ...component,
  render: (_) =>
    <TouchableOpacity
      onPress
      accessibilityLabel
      disabled
      style=Style.(style([flexDirection(Row), alignItems(Center)]))>
      <Icon name=icon color size=iconSize />
      <Text
        value=title
        style=(
          Style.style([
            Style.paddingLeft(Style.Pt(5.)),
            Style.color(color),
            Style.fontSize(Style.Float(float_of_int(fontSize))),
            Style.fontWeight(bold ? `Bold : `Normal)
          ])
        )
      />
    </TouchableOpacity>
};
