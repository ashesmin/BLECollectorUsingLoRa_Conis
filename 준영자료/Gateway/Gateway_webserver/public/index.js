var expIndex = 0; 	var Index = 0; 

window.onload = function() {

	setInterval("getClientTime()",1000)
	showExpControlPage();
}

 
function getClientTime(){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			document.getElementById("clientTime").innerHTML = this.responseText;
		}
	};

	xhttp.open("GET", "http://192.168.11.1:4000", true);
	xhttp.open("GET", "http://192.168.11.1:4000/systemtime", true);
	xhttp.send();
}
 

function showExpControlPage()
{

  document.getElementById("expWiFiPage").setAttribute("style", "display:none");
  document.getElementById("expLoRaPage").setAttribute("style", "display:none");

  document.getElementById("showExpControlPage").setAttribute("style", "")
}
 
 

function showExpLoRaPage()
{
  document.getElementById("showExpControlPage").setAttribute("style", "display:none");
  document.getElementById("expWiFiPage").setAttribute("style", "display:none");
 
  document.getElementById("expLoRaPage").setAttribute("style", "");

}

function showExpWiFiPage()
{
  document.getElementById("expLoRaPage").setAttribute("style", "display:none");
  document.getElementById("showExpControlPage").setAttribute("style", "display:none");
 
 document.getElementById("expWiFiPage").setAttribute("style", "");
}


function addLoRaExp(obj)
{
	

	var default_year = 2017;
	var default_month = 1;
	var default_day = 1;
	var default_hour = 0;
	var default_min = 0;
	var default_bw = 125;
	var default_cr = 5;
	var default_sf = 7;
	var default_mode =1;

	var default_src =1;
	var default_dst =0;
	var default_sqc =1;

	var borderSection = document.createElement("div");
		borderSection.className = "w3-section"

	borderSection.id = expIndex;
	Index =0 ;
	expIndex++;
	
	//showStartButton();
 
	var foldingButton = document.createElement("span");
	var fold = 0;
	foldingButton.className = "w3-center w3-left-margin w3-grey w3-ripple btn-default btn-lg glyphicon glyphicon-collapse-up";
	foldingButton.style.width = "60%";
	foldingButton.onclick = function() {
		brotherNodes = this.parentNode.childNodes;
		if(fold == 0) {
			for(var i=2;i<14+Index;i++)
				brotherNodes[i].style.display="none";
			this.className = "w3-center w3-grey w3-ripple btn-default btn-lg glyphicon glyphicon-collapse-down";
			this.style.width = "60%";
			fold = 1;
		} else {
			for(var i=2;i<14+Index;i++)
				brotherNodes[i].style.display="block";
			this.className = "w3-center w3-grey w3-ripple btn-default btn-lg glyphicon glyphicon-collapse-up";
			this.style.width = "60%";
			fold = 0;
		}
	};

	var removeButton = document.createElement("span");
		removeButton.style.width = "35%";
	removeButton.className = "w3-center w3-dark-grey w3-ripple btn-default btn-lg glyphicon glyphicon-trash w3-margin w3-";
	removeButton.onclick = function() {
		this.parentNode.remove();
	
		expIndex--;

	};

    var idSection = document.createElement("div");
	var idRest = document.createElement("div");
	var id_tooltip = document.createElement("span");
	var idInput = document.createElement("input");

	idSection.className = "w3-row w3-section";
	idRest.innerHTML = "실험 이름(Exp ID) : ";
	idRest.className = "w3-tooltip";
	id_tooltip.innerHTML = "실험 이름(Exp ID)는 10글자이내로 작성해주세요."
	id_tooltip.className = "w3-text";
	idInput.style.width = "100%";
	idInput.className = "w3-border";
	idSection.appendChild(idRest);
	idRest.appendChild(id_tooltip);
	idRest.appendChild(idInput);


	var intervalSection = document.createElement("div");
	var intervalRest = document.createElement("div");
	var interval_tooltip = document.createElement("span");
	var intervalInput = document.createElement("input");
	intervalSection.className = "w3-row w3-section";
	intervalRest.innerHTML = "패킷 당 전송 간격(Packet Interval) : ";
	intervalRest.className = "w3-tooltip";
	interval_tooltip.innerHTML = "패킷을 하나를 전송하고 난 뒤, 다음 패킷을 보내기까지의 간격을 설정합니다. ms 단위로 입력해주세요."
	interval_tooltip.className = "w3-text";
	intervalInput.style.width = "100%";
	intervalInput.className = "w3-border";
	intervalSection.appendChild(intervalRest);
	intervalRest.appendChild(interval_tooltip);
	intervalRest.appendChild(intervalInput);


	var transmissionSlotLengthSection = document.createElement("div");
	var transmissionSlotLengthRest = document.createElement("div");
	var transmissionSlotLength_tooltip= document.createElement("span");
	var transmissionSlotLengthInput = document.createElement("input");

	transmissionSlotLengthSection.className = "w3-row w3-section";
	
	transmissionSlotLengthRest.innerHTML = "전송 슬롯의 길이(Transmission Slot Length) : ";
	transmissionSlotLengthRest.className = "w3-tooltip";
   
    transmissionSlotLength_tooltip.innerHTML = "패킷을 전송하는 슬롯의 시간길이를 입력해주세요. ms 단위로 입력해주세요."
	transmissionSlotLength_tooltip.className = "w3-text";
	
	transmissionSlotLengthInput.style.width = "100%";
	transmissionSlotLengthInput.className = "w3-border";
	
	transmissionSlotLengthSection.appendChild(transmissionSlotLengthRest);
	
	transmissionSlotLengthRest.appendChild(transmissionSlotLength_tooltip);
	transmissionSlotLengthRest.appendChild(transmissionSlotLengthInput);


    var tranceiverInfoSection = document.createElement("div");
	var tranceiverInfoRest = document.createElement("div");
	var tranceiverInfo_tooltip = document.createElement("span");
	tranceiverInfoSection1 = document.createElement("div");

	

	tranceiverInfoSection.className = "w3-row w3-section w3-tooltip";
	tranceiverInfoRest.innerHTML = "송/수신 정보 (Tranceiver Info) : ";
	tranceiverInfoRest.className = "w3-rest";
    tranceiverInfo_tooltip.innerHTML = "송/수신 정보 (Tranceiver Info)는 발신주소/수신주소/전송순서로 입력하세요."
	tranceiverInfo_tooltip.className = "w3-text";


	tranceiverInfoRest.className = "w3-rest"
	

	tranceiverInfoSection.appendChild(tranceiverInfoRest);
	tranceiverInfoRest.appendChild(tranceiverInfo_tooltip);		
    
	

	var tranceiverAddButton = document.createElement("span");
	tranceiverAddButton.className = "btn btn-default  w3-padding-large btn-sm w3-ripple glyphicon glyphicon-collapse-lus";
	tranceiverAddButton.innerHTML =" Add "
 	var tranceiverAddInfoSection;
 	
 	var tranceiverAddInfoIndex = "tranceiverInfoIndex"+Index;
	
	tranceiverAddButton.onclick = function() {

	var tranceiverSrcRest = document.createElement("span");
		tranceiverSrcRest.className = "w3-text w3-section";
		tranceiverSrcRest.innerHTML = "  송신 주소  : ";
	var tranceiverSrc = document.createElement("select");
	
	var tranceiverDstRest = document.createElement("span");
		tranceiverDstRest.className = "w3-text w3-section";
		tranceiverDstRest.innerHTML = "   수신 주소 :  ";
	var tranceiverDst = document.createElement("select");

	var tranceiverSqcRest = document.createElement("span");
		tranceiverSqcRest.className = "w3-text w3-section";
		tranceiverSqcRest.innerHTML = "   전송 순서 :  ";
	var tranceiverSqc = document.createElement("select");

	//var expStartTimeHour = document.createElement("select");
	//var expStartTimeMinute = document.createElement("select");

	var src_option = new Array();
	var dst_option = new Array();
	var sqc_option = new Array();
	//var hour_option = new Array();
	//var min_option = new Array();

	for(var i=0;i<255;i++) {
		src_option[i] = document.createElement("option");
		src_option[i].text = default_src;
		tranceiverSrc.add(src_option[i]);
		default_src++;
	}
	default_src=1;
	
	for(var i=0;i<255;i++) {
		dst_option[i] = document.createElement("option");
		dst_option[i].text = default_dst;
		tranceiverDst.add(dst_option[i]);
		default_dst++;
	}
	default_dst=0;
	
	for(var i=0;i<255;i++) {
		sqc_option[i] = document.createElement("option");
		sqc_option[i].text = default_sqc;
		tranceiverSqc.add(sqc_option[i]);
		default_sqc++;
	}
	default_sqc= 1;

	tranceiverSrc.style.width = "24.5%";
	tranceiverSrc.className = "w3-border";
	tranceiverDst.style.width = "24.5%";
	tranceiverDst.className = "w3-border";
	tranceiverSqc.style.width = "24.5%";
	tranceiverSqc.className = "w3-border";
	

	tranceiverAddInfoSection = document.createElement("div");

	tranceiverAddInfoSection.appendChild(tranceiverSrcRest);
	tranceiverSrcRest.appendChild(tranceiverSrc);

	tranceiverAddInfoSection.appendChild(tranceiverDstRest);
	tranceiverDstRest.appendChild(tranceiverDst);

	tranceiverAddInfoSection.appendChild(tranceiverSqcRest);
	tranceiverSqcRest.appendChild(tranceiverSqc);

	tranceiverAddInfoSection.id = tranceiverAddInfoIndex;
	Index++;
	console.log("index : "+Index);
	console.log(tranceiverAddInfoIndex);
	borderSection.appendChild(tranceiverAddInfoSection);

};

	var tranceiverRemoveButton = document.createElement("span");
	tranceiverRemoveButton.className = "btn btn-default  w3-padding-large btn-sm w3-ripple glyphicon glyphicon-collapse-minus";
	tranceiverRemoveButton.innerHTML =" Remove "
	tranceiverRemoveButton.onclick = function() {
	 
	var tranceiverRemove = document.getElementById(tranceiverAddInfoIndex);
	tranceiverRemove.remove(tranceiverRemove);
		console.log(tranceiverAddInfoIndex);
	Index--;

	};

	var totalPacketCountSection = document.createElement("div");
	var totalPacketCountRest = document.createElement("div");
	var total_pacekt_count_tooltip = document.createElement("span");
	var totalPacketCountInput = document.createElement("input");
	totalPacketCountSection.className = "w3-row w3-section";
	totalPacketCountRest.innerHTML = "전체 패킷 갯수(Total Packet Count) : ";
	totalPacketCountRest.className = "w3-tooltip";
	total_pacekt_count_tooltip.innerHTML = "한 실험에서 전송할 패킷의 총 갯수를 입력해주세요.";
	total_pacekt_count_tooltip.className = "w3-text";
	totalPacketCountInput.style.width = "100%";
	totalPacketCountInput.className = "w3-border";
	totalPacketCountSection.appendChild(totalPacketCountRest);
	totalPacketCountRest.appendChild(total_pacekt_count_tooltip);
	totalPacketCountRest.appendChild(totalPacketCountInput);	

	var expStartTimeSection = document.createElement("div");
	var expStartTimeRest = document.createElement("div");
	var expEndTimeTooltip = document.createElement("span");
	var expStartTimeDateRest = document.createElement("div");

	var expStartTimeYear = document.createElement("select");
	var expStartTimeMonth = document.createElement("select");
	var expStartTimeDay = document.createElement("select");
	var expStartTimeHour = document.createElement("select");
	var expStartTimeMinute = document.createElement("select");

	var year_option = new Array();
	var month_option = new Array();
	var day_option = new Array();
	var hour_option = new Array();
	var min_option = new Array();

	for(var i=0;i<20;i++) {
		year_option[i] = document.createElement("option");
		year_option[i].text = default_year;
		expStartTimeYear.add(year_option[i]);
		default_year++;
	}

	for(var i=0;i<12;i++) {
		month_option[i] = document.createElement("option");
		month_option[i].text = default_month;
		expStartTimeMonth.add(month_option[i]);
		default_month++;
	}

	for(var i=0;i<31;i++) {
		day_option[i] = document.createElement("option");
		day_option[i].text = default_day;
		expStartTimeDay.add(day_option[i]);
		default_day++;
	}

	for(var i=0;i<24;i++) {
		hour_option[i] = document.createElement("option");

		if(default_hour < 10)
			hour_option[i].text = "0" + default_hour;
		else
			hour_option[i].text = default_hour + "";

		expStartTimeHour.add(hour_option[i]);
		default_hour++;
	}

	for(var i=0;i<60;i++) {
		min_option[i] = document.createElement("option");

		if(default_min < 10)
			min_option[i].text = "0" + default_min;
		else
			min_option[i].text = default_min + "";

		expStartTimeMinute.add(min_option[i]);
		default_min++;
	}

	expStartTimeYear.onchange = function() {
		var curr_exp_interval = intervalInput.value;
		var curr_exp_count = totalPacketCountInput.value;
		var curr_exp_slotlength =transmissionSlotLengthInput.value;
		var curr_exp_nodenum = Index


		var exp_end_time = calculateTime(expStartTimeYear.value, (expStartTimeMonth.value-1), expStartTimeDay.value, expStartTimeHour.value, expStartTimeMinute.value,curr_exp_interval, curr_exp_count, curr_exp_slotlength, curr_exp_nodenum);

		expEndTimeTooltip.innerHTML = exp_end_time;
	}

	expStartTimeMonth.onchange = function() {
		var curr_exp_interval = intervalInput.value;
		var curr_exp_count = totalPacketCountInput.value;
		var curr_exp_slotlength =transmissionSlotLengthInput.value;
		var curr_exp_nodenum = Index


		var exp_end_time = calculateTime(expStartTimeYear.value, (expStartTimeMonth.value-1), expStartTimeDay.value, expStartTimeHour.value, expStartTimeMinute.value,curr_exp_interval, curr_exp_count, curr_exp_slotlength, curr_exp_nodenum);

     	expEndTimeTooltip.innerHTML = exp_end_time;
	}

	expStartTimeDay.onchange = function() {
		var curr_exp_interval = intervalInput.value;
		var curr_exp_count = totalPacketCountInput.value;
		var curr_exp_slotlength =transmissionSlotLengthInput.value;
		var curr_exp_nodenum = Index

		var exp_end_time = calculateTime(expStartTimeYear.value, (expStartTimeMonth.value-1), expStartTimeDay.value, expStartTimeHour.value, expStartTimeMinute.value,curr_exp_interval, curr_exp_count, curr_exp_slotlength, curr_exp_nodenum);

		expEndTimeTooltip.innerHTML = exp_end_time;
	}

	expStartTimeHour.onchange = function() {
		var curr_exp_interval = intervalInput.value;
		var curr_exp_count = totalPacketCountInput.value;
		var curr_exp_slotlength =transmissionSlotLengthInput.value;
		var curr_exp_nodenum = Index


		var exp_end_time = calculateTime(expStartTimeYear.value, (expStartTimeMonth.value-1), expStartTimeDay.value, expStartTimeHour.value, expStartTimeMinute.value,curr_exp_interval, curr_exp_count, curr_exp_slotlength, curr_exp_nodenum);

		expEndTimeTooltip.innerHTML = exp_end_time;
	}

	expStartTimeMinute.onchange = function() {
		var curr_exp_interval = intervalInput.value;
		var curr_exp_count = totalPacketCountInput.value;
		var curr_exp_slotlength =transmissionSlotLengthInput.value;
		var curr_exp_nodenum = Index


		var exp_end_time = calculateTime(expStartTimeYear.value, (expStartTimeMonth.value-1), expStartTimeDay.value, expStartTimeHour.value, expStartTimeMinute.value,curr_exp_interval, curr_exp_count, curr_exp_slotlength, curr_exp_nodenum);

		expEndTimeTooltip.innerHTML = exp_end_time;
	}



	expStartTimeSection.className = "w3-row w3-section w3-tooltip";
	expStartTimeRest.innerHTML = "실험 시작 시간(Exp Start Time) : ";
	expStartTimeRest.className = "w3-rest";

	expEndTimeTooltip.innerHTML = "실험 종료 시간(Exp End Time) : " + expStartTimeYear.value + "년 " + expStartTimeMonth.value + "월 " + expStartTimeDay.value + "일 " + expStartTimeHour.value + "시 " + expStartTimeMinute.value + "분";
	expEndTimeTooltip.className = "w3-text";

	expStartTimeDateRest.className = "w3-rest"
	expStartTimeYear.style.width = "20%";
	expStartTimeYear.className = "w3-border";
	expStartTimeMonth.style.width = "20%";
	expStartTimeMonth.className = "w3-border";
	expStartTimeDay.style.width = "20%";
	expStartTimeDay.className = "w3-border";
	expStartTimeHour.style.width = "20%";
	expStartTimeHour.className = "w3-border";
	expStartTimeMinute.style.width = "20%";
	expStartTimeMinute.className = "w3-border";

	expStartTimeSection.appendChild(expStartTimeRest);
	expStartTimeSection.appendChild(expStartTimeDateRest);
	expStartTimeSection.appendChild(expEndTimeTooltip);
	expStartTimeDateRest.appendChild(expStartTimeYear);
	expStartTimeDateRest.appendChild(expStartTimeMonth);
	expStartTimeDateRest.appendChild(expStartTimeDay);
	expStartTimeDateRest.appendChild(expStartTimeHour);
	expStartTimeDateRest.appendChild(expStartTimeMinute);


	
	var modeSection = document.createElement("div");
	var modeRest = document.createElement("div");
	var mode_tooltip = document.createElement("span");
	var modeSelect = document.createElement("select");
	var mode_option = new Array();

	for(var i=0;i<10;i++) {
		mode_option[i] = document.createElement("option");
		mode_option[i].text = default_mode;
		modeSelect.add(mode_option[i]);
		default_mode++;
	}

	modeSection.className = "w3-row w3-section";
	modeRest.innerHTML = "모드(Mode) : ";
	modeRest.className = "w3-tooltip";
	mode_tooltip.innerHTML = "모드(Mode)가 높으면 전송 속도(Data Rate)가 빠르지만, 최대 허용수신감도(Sensitivity)가 떨어집니다."
	mode_tooltip.className = "w3-text";
	modeSelect.style.width = "100%";
	modeSelect.className = "w3-border w3-input";
	
	modeSection.appendChild(modeRest);
	modeRest.appendChild(mode_tooltip);		
	modeRest.appendChild(modeSelect);		

	var powerSection = document.createElement("div");
	var powerRest = document.createElement("div");
	var powerTooltip = document.createElement("span");
	var powerSelect = document.createElement("select");
	var power_option = new Array();

	for(var i=0;i<3;i++)
		power_option[i] = document.createElement("option");

	power_option[0].text = "Max";
	power_option[1].text = "High";
	power_option[2].text = "Low";

	for(var i=0;i<3;i++)
		powerSelect.add(power_option[i]);
	
	powerSection.className = "w3-row w3-section";
	powerRest.innerHTML = "파워(Power) : ";
	powerRest.className = "w3-tooltip";
	powerTooltip.innerHTML = "Max - 14dBm / High - 7dBm / Low - 0dBm 의 출력으로 패킷을 전송합니다. 파워는 전력 소모(Power Consumption)에 영향을 끼칩니다."
	powerTooltip.className = "w3-text";
	powerSelect.style.width = "100%";
	powerSelect.className = "w3-border w3-input";

	powerSection.appendChild(powerRest);
	powerRest.appendChild(powerTooltip);		
	powerRest.appendChild(powerSelect);

	var channelSection = document.createElement("div");
	var channelRest = document.createElement("div");
	var channelSelect = document.createElement("select");
	var ch_option = new Array();

	for(var i=0;i<13;i++)
		ch_option[i] = document.createElement("option");

	ch_option[0].text = "00 (903.08MHz)";
	ch_option[1].text = "01 (905.24MHz)";
	ch_option[2].text = "02 (907.40MHz)";
	ch_option[3].text = "03 (909.56MHz)";
	ch_option[4].text = "04 (911.72MHz)";
	ch_option[5].text = "05 (913.88MHz)";
	ch_option[6].text = "06 (916.04MHz)";
	ch_option[7].text = "07 (918.20MHz)";
	ch_option[8].text = "08 (920.36MHz)";
	ch_option[9].text = "09 (922.52MHz)";
	ch_option[10].text = "10 (924.68MHz)";
	ch_option[11].text = "11 (926.84MHz)";
	ch_option[12].text = "12 (915MHz)";

	for(var i=0;i<13;i++)
	channelSelect.add(ch_option[i]);
	
	channelSection.className = "w3-row w3-section";
	channelRest.innerHTML = "채널(CH) : ";
	channelRest.className = "w3-rest";
	channelSelect.style.width = "100%";
	channelSelect.className = "w3-border w3-input";

	channelSection.appendChild(channelRest);		
	channelRest.appendChild(channelSelect);

	borderSection.appendChild(foldingButton);
	borderSection.appendChild(removeButton);
	borderSection.appendChild(idSection);
	borderSection.appendChild(intervalSection);
	borderSection.appendChild(totalPacketCountSection);
	borderSection.appendChild(transmissionSlotLengthSection);

	//borderSection.appendChild(transmissionSequenceSection);
	
	
	borderSection.appendChild(expStartTimeSection);
	borderSection.appendChild(modeSection);
	borderSection.appendChild(powerSection);
	borderSection.appendChild(channelSection);

	borderSection.appendChild(tranceiverInfoSection);
	borderSection.appendChild(tranceiverAddButton);
	borderSection.appendChild(tranceiverRemoveButton);
    
	obj.parentNode.parentNode.insertBefore(borderSection, obj.parentNode.parentNode.lastElementChild);

}


function setConfiguration()
{
  /*
    var config_data = new Object();
	var exp_array = new Array();
	var lora_config_array = new Array();
	var lora_traffic = new Object();
	var lora_send_info = new Array();
	var lora_parameter = new Object();
*/
var timeData;
 var monthData;
 var dayData;
 var emptySpace = "0";
var expArray= { exps: [ /* { LoRa: Lora, WiFi: WiPi }*/ ], loraExpNum: null,wifiExpNum: null };
for(var i=0;i<expIndex;i++) {
  		var curIndex = i.toString();
  		var curExp = document.getElementById(curIndex);



		var expData = { exp: [ /* { LoRa: Lora, WiFi: WiPi }*/ ]};
		var wifiArray = { WiFi: [ /* { ExpParam: ExpParameter, ExpTraff: ExpTraffic } */ ] };
		var loraArray = { LoRa: [ /* { ExpParam: ExpParameter, ExpTraff: ExpTraffic } */ ] };
		var loraExpTraffic = { expId : null, packetInterval :null, packetNum: null, sendSlotLength: null, startTime: null,
						sendTranceiverInfo: [ /* { SendTraceiverInfo : SendTraceiverInfo } */ ]};
		var loraExpParameter = { Channel: null, Mode: null, Power: null };

		var sendTranceiverInfo = { src: null, dst: null, sqc: null };
			

			if(Number(curExp.childNodes[6].childNodes[1].childNodes[1].value) < 10)
		  			monthData = emptySpace + curExp.childNodes[6].childNodes[1].childNodes[1].value;
		  		else
		  			monthData = curExp.childNodes[6].childNodes[1].childNodes[1].value;

		  		if(Number(curExp.childNodes[6].childNodes[1].childNodes[2].value) < 10)
		  			dayData = emptySpace + curExp.childNodes[6].childNodes[1].childNodes[2].value;
		  		else
		  			dayData = curExp.childNodes[6].childNodes[1].childNodes[2].value;

		  		loraExpTraffic["expId"] = curExp.childNodes[2].childNodes[0].childNodes[2].value;
			  	loraExpTraffic["packetInterval"] = Number(curExp.childNodes[3].childNodes[0].childNodes[2].value);
			  	loraExpTraffic["packetNum"] =Number(curExp.childNodes[4].childNodes[0].childNodes[2].value);
			  	loraExpTraffic["sendSlotLength"] = Number(curExp.childNodes[5].childNodes[0].childNodes[2].value);
			    loraExpTraffic["startTime"] =  curExp.childNodes[6].childNodes[1].childNodes[0].value + monthData + dayData + " " + curExp.childNodes[6].childNodes[1].childNodes[3].value + ":" + curExp.childNodes[6].childNodes[1].childNodes[4].value + ":00";
			  	
  		
			  	
			  
			 //  expTraffic["sendTraceiverInfo"].push(sendTranceiverInfo);

			  	loraExpParameter["Mode"] =  Number(curExp.childNodes[7].childNodes[0].childNodes[2].value);
			  	loraExpParameter["Power"] =  curExp.childNodes[8].childNodes[0].childNodes[2].value.substring(0,1);
			 	loraExpParameter["Channel"] =  "CH_"+ curExp.childNodes[9].childNodes[0].childNodes[1].value.substring(0,2)+"_900";


			 for(var j = 0; j<Index; j++)
			 {
			 	sendTranceiverInfo = new Object({ src: null, dst: null, sqc: null });

			 	sendTranceiverInfo["src"] = Number(curExp.childNodes[13+j].childNodes[0].childNodes[1].value);
				sendTranceiverInfo["dst"] = Number(curExp.childNodes[13+j].childNodes[1].childNodes[1].value);
				sendTranceiverInfo["sqc"] = Number(curExp.childNodes[13+j].childNodes[2].childNodes[1].value);	  	 
				loraExpTraffic["sendTranceiverInfo"].push(sendTranceiverInfo);
				
			 }
				expData["exp"].push(wifiArray);

				loraArray["LoRa"].push(loraExpParameter);
				loraArray["LoRa"].push(loraExpTraffic);
				expData["exp"].push(loraArray);
	 
 
				expArray["exps"].push(expData);
	
	  
	}
				expArray["loraExpNum"] = expIndex;
    			console.log(JSON.stringify(expArray));
		 
		 	var url = "http://192.168.11.1:4000/exps"; //gateway
		    var xhttp = new XMLHttpRequest();

		    xhttp.onreadystatechange = function() {
		      if (this.readyState == 4 && this.status == 200) {
		      	alert(xhttp.responseText);
		      }
		    };
		    xhttp.open("POST", url, true);
		    xhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
		    xhttp.send(JSON.stringify(expArray));
}



function stopExp() {
	var url = "http://192.168.11.1:4000/exps"; //gateway address

    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
      	alert(xhttp.responseText);
      }
    };
    xhttp.open("GET", url, true);
    xhttp.send(null);

  
}

function setDeviceTime() {
	var url = "http://192.168.11.1:4000/exps"; //gateway
    var xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
      }
    };

    var time_obj = new Object();
    time_obj.Time = getTimeStamp();


    xhttp.open("PUT", url, true);
    xhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xhttp.send(JSON.stringify(time_obj));
}


function getTimeStamp() {
	var d = new Date();
	var s =
	leadingZeros(d.getFullYear(), 4) + leadingZeros(d.getMonth() + 1, 2) + leadingZeros(d.getDate(), 2) + ' ' +
    leadingZeros(d.getHours(), 2) + ':' +
    leadingZeros(d.getMinutes(), 2) + ':' +
    leadingZeros(d.getSeconds(), 2);

    return s;
}

function leadingZeros(n, digits) {
	var zero = '';
	n = n.toString();

	if (n.length < digits) {
		for (i = 0; i < digits - n.length; i++)
			zero += '0';
	}

	return zero + n;
}

function calculateTime(year, mon, day, hour, min, int, cnt, lng, num) {
	var exp_start_time = new Date(year, mon, day, hour, min, 0);
	var total_interval = (Number(int)/1000+Number(lng)/1000) * Number(cnt) * Number(num);
	
	exp_start_time.setSeconds(exp_start_time.getSeconds() + total_interval);

	var exp_end_year = exp_start_time.getFullYear();
	var exp_end_month = exp_start_time.getMonth() + 1;
	var exp_end_day = exp_start_time.getDate();
	var exp_end_hour = exp_start_time.getHours();
	var exp_end_minute = exp_start_time.getMinutes();
	var exp_end_second = exp_start_time.getSeconds();

	var exp_end_time = "실험 종료시간(Exp End Time) : " + exp_end_year + "년 " + exp_end_month + "월 " + exp_end_day + "일 " + exp_end_hour + "시 " + exp_end_minute + "분 " + exp_end_second + "초";
	return exp_end_time;
}



 
