open BsReactNative;

open Js;

let component = ReasonReact.statelessComponent("Talk");

let styles =
  StyleSheet.create(
    Style.(
      {
        "track":
          style([
            fontWeight(`Bold),
            fontSize(Float(13.)),
            paddingLeft(Pt(5.)),
            paddingTop(Pt(20.)),
            color(Theme.textMediumColor)
          ]),
        "title":
          style([
            fontSize(Float(25.)),
            fontWeight(`Bold),
            paddingTop(Pt(3.)),
            paddingLeft(Pt(5.)),
            paddingRight(Pt(5.))
          ]),
        "subtitle":
          style([
            fontSize(Float(16.)),
            color(Theme.subtitleColor),
            paddingTop(Pt(5.)),
            paddingLeft(Pt(5.)),
            paddingRight(Pt(5.))
          ]),
        "info":
          style([
            flexDirection(Row),
            justifyContent(SpaceBetween),
            paddingLeft(Pt(10.)),
            paddingRight(Pt(10.)),
            paddingTop(Pt(15.))
          ]),
        "infoTitle":
          style([
            fontSize(Float(12.)),
            color(Theme.textLighterColor),
            fontWeight(`Bold)
          ]),
        "infoValue":
          style([fontSize(Float(13.)), color(Theme.textMediumColor)]),
        "actions":
          style([
            flexDirection(Row),
            paddingLeft(Pt(15.)),
            paddingRight(Pt(15.)),
            paddingTop(Pt(5.)),
            paddingBottom(Pt(15.)),
            alignItems(Center),
            borderBottomWidth(1.),
            borderBottomColor(Theme.lightBorderColor),
            justifyContent(SpaceBetween)
          ]),
        "saveExplain":
          style([
            fontWeight(`Bold),
            fontSize(Float(11.)),
            paddingLeft(Pt(10.)),
            color(Theme.textLightColor)
          ]),
        "sectionTitle":
          style([
            fontSize(Float(20.)),
            fontWeight(`Bold),
            paddingTop(Pt(20.)),
            paddingBottom(Pt(10.))
          ]),
        "section": style([paddingLeft(Pt(10.)), paddingRight(Pt(10.))])
      }
    )
  );

let htmlList = (f: 'a => string, elems: array('a)) =>
  <HtmlText
    value=(
      "<ul>"
      ++ Array.reduce(
           (++),
           "",
           Array.map(elem => "<li>" ++ f(elem) ++ "</li>", elems)
         )
      ++ "</ul>"
    )
    styles=Theme.htmlStyles
  />;

let infoSection = (title: string, value: string) =>
  <View>
    <Text style=styles##infoTitle value=title />
    <Text style=styles##infoValue value />
  </View>;

let share = (talk: Schedule.Event.t) =>
  Share.(
    share(
      content(
        ~message=talk.title ++ " #fosdem18",
        ~url="https://fosdem.org/2018/schedule/event/" ++ talk.slug,
        ()
      ),
      options(~tintColor="rgb(119,81,251)", ())
    )
    |> ignore
  );

let make =
    (
      ~talk: Schedule.Event.t,
      ~favorite: int => unit,
      ~removeFavorite: int => unit,
      ~navigation,
      _
    ) => {
  ...component,
  render: (_) =>
    <View style=Theme.styles##container>
      <View style=Theme.styles##nav>
        <IconButton
          title="Back"
          icon="arrow-left"
          onPress=(() => StackNavigator.goBack(navigation))
          fontSize=16
          iconSize=25
          color=Theme.accent
        />
      </View>
      <ScrollView style=Theme.styles##scrollview>
        <View style=Theme.styles##header>
          <Text value=(String.toUpperCase(talk.track)) style=styles##track />
          <Text value=talk.title style=styles##title />
          (
            talk.subtitle != "" ?
              <Text value=talk.subtitle style=styles##subtitle /> :
              ReasonReact.nullElement
          )
        </View>
        <View style=styles##actions>
          (
            talk.favorite ?
              <IconButton
                title="REMOVE FAVORITE"
                kind=Icon.entypo
                icon="heart"
                onPress=(() => removeFavorite(talk.id))
                iconSize=20
                fontSize=11
                bold=true
                color="rgb(119,81,251)"
              /> :
              <IconButton
                title="FAVORITE"
                kind=Icon.entypo
                icon="heart-outlined"
                onPress=(() => favorite(talk.id))
                iconSize=20
                fontSize=11
                bold=true
                color="rgb(119,81,251)"
              />
          )
          <IconButton
            title="SHARE"
            icon="share"
            iconSize=20
            onPress=(() => share(talk))
            fontSize=11
            bold=true
            color="rgb(119,81,251)"
          />
        </View>
        <View style=styles##info>
          (infoSection("Day", Dates.dayName(talk.date)))
          (infoSection("Hour", Schedule.Time.toString(talk.start)))
          (infoSection("Duration", string_of_int(talk.duration) ++ " mins"))
          (infoSection("Room", talk.room))
        </View>
        (
          talk.abstract != "" ?
            <View style=styles##section>
              <Text style=styles##sectionTitle value="Abstract" />
              <HtmlText
                value=("<div>" ++ talk.abstract ++ "</div>")
                styles=Theme.htmlStyles
              />
            </View> :
            ReasonReact.nullElement
        )
        (
          talk.description != "" ?
            <View style=styles##section>
              <Text style=styles##sectionTitle value="Description" />
              <HtmlText
                value=("<div>" ++ talk.description ++ "</div>")
                styles=Theme.htmlStyles
              />
            </View> :
            ReasonReact.nullElement
        )
        (
          Array.length(talk.people) > 0 ?
            <View style=styles##section>
              <Text style=styles##sectionTitle value="People" />
              (htmlList((p: Schedule.Person.t) => p.name, talk.people))
            </View> :
            ReasonReact.nullElement
        )
        (
          Array.length(talk.links) > 0 ?
            <View style=styles##section>
              <Text style=styles##sectionTitle value="Links" />
              (
                htmlList(
                  (l: Schedule.Link.t) =>
                    "<a href=\"" ++ l.url ++ "\">" ++ l.title ++ "</a>",
                  talk.links
                )
              )
            </View> :
            ReasonReact.nullElement
        )
      </ScrollView>
    </View>
};

let jsComponent =
  ReasonReact.wrapReasonForJs(
    ~component,
    jsProps => {
      let talkId = int_of_string(jsProps##navigation##state##params##talk);
      let talk = Schedule.findTalk(jsProps##screenProps##schedule, talkId);
      make(
        ~talk,
        ~navigation=StackNavigator.navigation(jsProps),
        ~favorite=jsProps##screenProps##favorite,
        ~removeFavorite=jsProps##screenProps##removeFavorite,
        [||]
      );
    }
  );