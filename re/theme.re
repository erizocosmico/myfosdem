open BsReactNative;

let accent = "rgb(119,81,251)";

let bodyTextColor = "rgba(0, 0, 0, .8)";

let textLightColor = "rgba(0, 0, 0, .6)";

let lightBorderColor = "rgba(0, 0, 0, .05)";

let textMediumColor = "rgba(0, 0, 0, .7)";

let textLighterColor = "rgba(0,0,0,.3)";

let subtitleColor = "rgba(0, 0, 0, .5)";

let tabBarTintColor = "rgba(57,57,57,0.7)";

let htmlStyles =
  StyleSheet.create(
    Style.(
      {
        "div": style([lineHeight(10.), margin(Pt(0.)), padding(Pt(0.))]),
        "ul": style([lineHeight(0.), margin(Pt(0.)), padding(Pt(0.))]),
        "li": style([lineHeight(0.), margin(Pt(0.)), padding(Pt(0.))]),
        "p":
          style([
            fontSize(Float(14.)),
            marginTop(Pt(0.)),
            marginBottom(Pt(0.)),
            lineHeight(20.),
            paddingTop(Pt(0.)),
            paddingBottom(Pt(0.)),
            color(bodyTextColor)
          ]),
        "a": style([color(accent), textDecorationLine(Underline)])
      }
    )
  );

let styles =
  StyleSheet.create(
    Style.(
      {
        "container": style([flex(1.), backgroundColor("white")]),
        "nav":
          style([
            paddingTop(Pt(25.)),
            paddingLeft(Pt(10.)),
            paddingBottom(Pt(10.))
          ]),
        "scrollview": style([flex(1.)]),
        "backBtn": style([paddingBottom(Pt(20.)), paddingLeft(Pt(15.))]),
        "header":
          style([
            paddingLeft(Pt(10.)),
            paddingRight(Pt(10.)),
            paddingBottom(Pt(10.))
          ]),
        "title":
          style([
            fontSize(Float(35.)),
            fontWeight(`Bold),
            paddingTop(Pt(20.)),
            paddingLeft(Pt(15.)),
            paddingBottom(Pt(15.))
          ]),
        "row":
          style([
            paddingLeft(Pt(15.)),
            paddingRight(Pt(15.)),
            paddingTop(Pt(10.)),
            paddingBottom(Pt(10.)),
            borderBottomWidth(1.),
            borderBottomColor(lightBorderColor)
          ]),
        "statusBar": style([height(Pt(20.)), backgroundColor("white")]),
        "searchBar":
          style([
            padding(Pt(8.)),
            borderRadius(10.),
            margin(Pt(15.)),
            backgroundColor(lightBorderColor),
            flexDirection(Row),
            alignItems(Center),
            justifyContent(SpaceBetween)
          ]),
        "searchInput":
          style([flex(1.), marginLeft(Pt(5.)), marginRight(Pt(5.))])
      }
    )
  );
