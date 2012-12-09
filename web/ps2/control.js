		var gamepadActive = false;
		var gamepadActive = false;
    ps2Buttons = new Array();
 
		ps2Buttons[0]  = 'triangle',
		ps2Buttons[3]  = 'square',
		ps2Buttons[2]  = 'cross',
		ps2Buttons[1]  = 'circle',
		ps2Buttons[6]  = 'L1',
		ps2Buttons[4]   = 'L2',
		ps2Buttons[7]  = 'R1',
		ps2Buttons[5]   = 'R2',
		ps2Buttons[8]   = 'select',
		ps2Buttons[9]   = 'start';
		 
		function gamepadConnected(evt)
		{
		  console.log(evt);
		  gamepadActive = true;
		}
		function gamepadDisconnected(evt)
		{
		  console.log(evt);
		  gamepadActive = false;
		}
		 
		window.addEventListener('MozGamepadConnected', gamepadConnected);
		window.addEventListener('MozGamepadDisconnected', gamepadDisconnected); 
		

		function buttonPressed(evt, pressed)
		{
		  //console.log(evt.button, pressed); 
		  //console.log(evt.button + ps2Buttons[evt.button] + pressed);
		  if(pressed){
		  	if(ps2Buttons[evt.button] == 'triangle')
		  		sendSer('U');
		  	if(ps2Buttons[evt.button] == 'cross')
		  		sendSer('D');		  	
		  }
		}
		window.addEventListener("MozGamepadButtonDown", function(evt) { buttonPressed(evt, true); } );
		window.addEventListener("MozGamepadButtonUp", function(evt) { buttonPressed(evt, false); } );
		
		

		function getDirection(x,y){
				var ret;
				
				/* center */
				if((x < 0.5) && (x > -0.5) && (y < 0.5) && (y > -0.5)){
					ret = 'C';
				}
				/* up or down*/
				else if((x < 0.5) && (x > -0.5)){
					if(y < -0.5)//up
						ret = 'U';
					else if(y > 0.5)//down
						ret = 'D';

				}
				/* left or right*/
				else if((y < 0.5) && (y > -0.5)){
					if(x < -0.5)//left
						ret = 'L';
					else if(x > 0.5)//right
						ret = 'R';
				}	
				

				return ret;
		}
		function update(){
		  /* only for PS2 controller */
      var pads = Gamepad.getStates();
      var pad = pads[0];//onlu one?
      if(pad){
      	//console.log(pad.leftStickX, pad.leftStickY);
				var flag = getDirection(pad.leftStickX,pad.leftStickY);
				switch(flag){
					case 'U':
						sendSer('F');
						//timeout = setTimeout(update(),500);
						break;
					case 'D':
						sendSer('B');
						//timeout = setTimeout(update(),500);
						break;
					case 'L':
						sendSer('L');
						//timeout = setTimeout(update(),500);
						break;
					case 'R':
						sendSer('R');
						//timeout = setTimeout(update(),500);
						break;
					case 'C':
						sendSer('S');
						//clearTimeout(timeout);
						break;
					default:
						//clearTimeout(timeout);
						break;
				}	
				

		    
		}
	}
		function moveAnalogSticks(evt) {
			
		  //timeout = setTimeout(update(),500);
		  update();



      

		}
		
		window.addEventListener("MozGamepadAxisMove", moveAnalogSticks); 
		
			
	function sendSer(value){
		console.log(value);
	document.getElementById("ser").src="http://192.168.3.10/cgi-bin/web2ser?"+value;
	}