var baseDP = "javascript.0.REMINDER.Post.";

createState(baseDP + "state", false);
createState(baseDP + "timestamp_open","");
createState(baseDP + "timestamp_receive","");



on({id:"mqtt.0.LetterBox.top_isOpen", change: "ne"}, function() { 
  var val = getState("mqtt.0.LetterBox.top_isOpen").val;

  if ( val === 1) {
    sendTo('telegram.0', {text: 'Post ist da', chatId: '513xxxxx'});
    setState(baseDP + "state", true); 
    var date = new Date();
    setState(baseDP + "timestamp_open", formatDate(date, "TT.MM.JJJJ SS:mm"));
    setState("alexa2.0.Echo-Devices.G2A0P30880140ABC.Commands.speak","Traaaari traaaara die Post ist da");
  }
});

on({id:"mqtt.0.LetterBox.front_isOpen", change: "ne"}, function() { 
  var val = getState("mqtt.0.LetterBox.front_isOpen").val;

  if ( val === 1) {
    sendTo('telegram.0', {text: 'Post wurde rausgeholt.', chatId: '513xxxxx'});
    var date = new Date();
    setState(baseDP + "timestamp_receive", formatDate(date, "TT.MM.JJJJ SS:mm"));
    setState(baseDP + "state", false); 
  }
});
