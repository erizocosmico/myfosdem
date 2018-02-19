open BsReactNative;

let component = ReasonReact.statelessComponent("SearchBar");

let make = (~onChangeText: string => unit, query: string) => {
  ...component,
  render: (_) =>
    <View style=Theme.styles##searchBar>
      <Icon name="search" color=Theme.textLightColor size=20 />
      <TextInput
        placeholder="Search"
        value=query
        style=Theme.styles##searchInput
        onChangeText
        autoCapitalize=`none
      />
      (
        query != "" ?
          <IconButton
            icon="delete"
            color=Theme.textLightColor
            onPress=(() => onChangeText(""))
            iconSize=20
            title=""
            fontSize=0
          /> :
          ReasonReact.nullElement
      )
    </View>
};
