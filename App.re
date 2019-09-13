open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let animatedText = {
  let component = React.component("AnimatedText");

  (~children as _: list(React.syntheticElement), ~delay, ~textContent, ()) =>
    component(hooks => {
      let (translate, hooks) =
        Hooks.animation(
          Animated.floatValue(50.),
          Animated.options(
            ~toValue=0.,
            ~duration=Seconds(0.5),
            ~delay=Seconds(delay),
            (),
          ),
          hooks,
        );

      let (opacityVal: float, hooks) =
        Hooks.animation(
          Animated.floatValue(0.),
          Animated.options(
            ~toValue=1.0,
            ~duration=Seconds(1.),
            ~delay=Seconds(delay),
            (),
          ),
          hooks,
        );

      let textHeaderStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(24),
          marginHorizontal(8),
          opacity(opacityVal),
          transform([Transform.TranslateY(translate)]),
        ];

      (hooks, <Text style=textHeaderStyle text=textContent />);
    });
};

let simpleButton = {
  let component = React.component("SimpleButton");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let (count, setCount, hooks) = React.Hooks.state(0, hooks);
      let increment = () => setCount(count + 1);

      let wrapperStyle =
        Style.[
          backgroundColor(Color.rgba(1., 1., 1., 0.1)),
          border(~width=2, ~color=Colors.white),
          margin(16),
        ];

      let textHeaderStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(20),
          margin(4),
        ];

      let textContent = "Click me: " ++ string_of_int(count);
      (
        hooks,
        <Clickable onClick=increment>
          <View style=wrapperStyle>
            <Text style=textHeaderStyle text=textContent />
          </View>
        </Clickable>,
      );
    });
};

let componentWithHooks = {
  let component = React.component("componentWithHooks");

  (~children: list(React.syntheticElement), ()) =>
    component(hooks => {
      let (currentTask, setTask, hooks) = Hooks.state("", hooks);
      let (taskList, addTask, hooks) = Hooks.state([], hooks);

      let (translate, hooks) =
        Hooks.animation(
          Animated.floatValue(100.),
          Animated.options(
            ~toValue=100.,
            ~duration=Seconds(1.0),
            ~repeat=true,
            (),
          ),
          hooks,
        );

      let _ = Style.[transform([Transform.TranslateY(translate)])];

      let tStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(14),
          marginHorizontal(8),
          transform([Transform.TranslateY(translate)]),
        ];

      let containerStyle = Style.[marginBottom(10)];

      let items =
        List.mapi(
          (i, task) => <Text style=tStyle text={"* " ++ task} />,
          taskList,
        );

      (
        hooks,
        <View>
          <Input
            value=currentTask
            onChange={e => setTask(e.value)}
            onKeyDown={e =>
              switch (e.key) {
              | Revery_Core.Key.KEY_ENTER =>
                addTask(taskList @ [currentTask]);
                setTask("");
              | _ => ()
              }
            }
          />
          <View style=containerStyle> ...items </View>
        </View>,
      );
    });
};

let init = app => {
  let win = App.createWindow(app, "Welcome to Hell!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

  let innerStyle = Style.[flexDirection(`Row), alignItems(`FlexEnd)];

  let element =
    <View style=containerStyle>
      <View style=innerStyle>
        <animatedText delay=0.0 textContent="Welcome" />
        <animatedText delay=1. textContent="to" />
        <animatedText delay=2. textContent="Hell" />
      </View>
      <componentWithHooks />
    </View>;

  let _ = UI.start(win, element);
  ();
};

App.start(init);