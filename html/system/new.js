$(document).ready(function(){
//精准获取小数点后两位	
function round2(number,fra){ 
   	with(Math){  
       	return round(number*pow(10,fra))/pow(10,fra);  
 	}  
};
//利率的折扣计算
function toFixeds(nums){	
	var vals = round2((nums),2);
	return vals;
};
//判断按揭年数选择
function chooseYears(ear,e){
	var index = 0;
	if (ear > 4) {
		if (e == 1) {
			index = 1;
		}else if (e > 1 && e < 4) {
			index = 2;
		}else if (e > 3 && e < 6) {
			index = 4;
		}else{
			index = 6;
		}
	}else if (ear == 4) {
		if (e == 1) {
			index = 1;
		}else if (e > 1 && e < 6) {
			index = 2;
		}else{
			index = 6;
		}
	}else if (ear < 4) {
		if (e < 6) {
			index = 1;
		}else{
			index = 6;
		}
	}
	return index;
}

lilvEnter=function(){
	var shang = $("#shang"),
		gong = $("#gong"),
		loanValue = document.getElementById("loan").value,	//html中贷款利率的value
		yearsIndex = ($("#years").get(0).selectedIndex)+1,	//按揭年数的当前被点击的索引值
		yearIn = chooseYears(loanValue,yearsIndex),			//chooseYears函数判断当前点击的索引，然后返回数组的key值
		dkStyle = ($("#loan").get(0).selectedIndex)+1,		//贷款利率的当前被点击的索引值
		lilvShang = toFixeds((lilv_array[dkStyle][1][yearIn]) * 100),	//被点击的key值，返回数组中相应的value值
		lilvGong = toFixeds((lilv_array[dkStyle][2][yearIn]) * 100);

	if (shang.hasClass('radio_yes')) {
		$("#rate").val(lilvShang);
	}else if (gong.hasClass('radio_yes')) {
		$("#rate").val(lilvGong);
	};
};
//验证是不是数字
function reg_Num(str){
	if (str.length == 0) {
		return false;
	};
	var Letters = "1234567890.";

	for (var i = 0; i < str.length; i++){
		var CheckChar = str.charAt(i);
		if (Letters.indexOf(CheckChar) == -1) {
			return false;
		};
	}
	return true;
};
//开始计算按妞
//模式切换
calc=function(){

	var shang = $("#shang"),
		gong =  $("#gong"),
		xi = $("#xi"),
		jin = $("#jin"),
		mon = $("#mon"),	//单价
		area = $("#area"),	//面积
		loanTotal = $("#loanTotal");	//贷款总额

	//检测输入框是否填写
	if ($("#total").hasClass('radio_no')) {
		if (!reg_Num(mon.val())) {
			alert("请填写单价");
			mon.focus();
			return;
		}else if (!reg_Num(area.val())) {
			alert("请填写面积");
			area.focus();
			return;
		};
	}else if ($("#total").hasClass('radio_yes')) {
		if (!reg_Num(loanTotal.val())) {
			alert("请填写贷款总额");
			loanTotal.focus();
			return;
		};
	};

	//贷款总额被选中时,等额本息和等额本金
	if ($("#total").hasClass('radio_yes')) {
		if (xi.hasClass('radio_yes')) {
			creditXi();
			return;
		}else if (jin.hasClass('radio_yes')) {
			creditJin();
			return;
		}
	};

	//检测贷款类别和还款方式
	if (shang.hasClass('radio_yes') || gong.hasClass('radio_yes')){
		if (xi.hasClass('radio_yes')) {
			calcBenXi();
			return;
		}else if (jin.hasClass('radio_yes')) {
			averBenJin();
			return;
		}
	};

}

/*等额本息*/
calcBenXi=function(){
	var M = $("#mon").val();	//单价
	var U = $("#area").val();	//面积
	var L = $("#rate").val();	//贷款利率
	var mor = $("#mor option:selected").val();		//按揭成数取值
	var x = $("#years option:selected").val();		//按揭年数取值
	var Y = x * 12;
	var MU = M * U;			//房款总数计算
	var R = (0.01 * L) * x / Y;			//月利率
	var mumor = MU * mor;		//贷款总额
	var pow = Math.pow((1 + R),Y);
	var top = mumor * R * pow;
	var down = pow - 1;
	var mavg = top/down;
	var refund = mavg * Y;		//还款总额
	var interest = refund - (MU * mor);		//支付利息款

	$("#MU").val(MU);		//房贷总额
	$("#loanMU").val((MU * mor).toFixed(2));		//贷款总额
	$("#refund").val((refund).toFixed(2));		//还款总额
	$("#interest").val((interest).toFixed(2));		//支付利息款
	$("#Down").val((MU - (MU * mor)).toFixed(2));		//首付
	$("#Y").val(Y + "(月)");	//贷款月数
	$("#mavg").val((mavg).toFixed(2));	//月均还款

};


/*等额本金*/
averBenJin=function(){

	var M = $("#mon").val();	//单价
	var U = $("#area").val();	//面积
	var L = $("#rate").val();	//贷款利率
	var mor = $("#mor option:selected").val();		//按揭成数取值
	var x = $("#years option:selected").val();		//按揭年数取值
	var Y = x * 12;
	var MU = M * U;			//房款总数计算
	var R = (0.01 * L) * x / Y;			//月利率
	var mumor = MU * mor;		//贷款总额
	var monthJin = mumor/Y;		//每月本金
	var totalInt = (Y + 1)*mumor*R/2;	//支付利息款
	var month = '';
	for(var i=0; i < Y;i++){
		var monthHuan = monthJin * (i);		//累计已还本金
		var monthM = ((mumor/Y) + ((mumor - monthHuan) * R)).toFixed(2);
		month += (i + 1) + "月," + monthM + "(元)\n";
	};
	

	$("#MU").val(MU);	//房款总额
	$("#loanMU").val((MU * mor).toFixed(2));	//贷款总额
	$("#refund").val(((MU * mor) + totalInt).toFixed(2));		//还款总额
	$("#interest").val((totalInt).toFixed(2));	//支付利息款
	$("#Down").val((MU - (MU * mor)).toFixed(2));	//首期付款
	$("#Y").val(Y + "(月)");		//贷款月数
	$("#mavg1").val(month); 		//月均还款

};


//重新填写按钮
reset=function(){
	$(".ul_li input").val("");
	$("#total input").val("");
	$(".rightShow input").val("");
	$("textarea").val("");
};

/*贷款总额,本息计算*/
creditXi=function(){

	var loanTotal = $("#loanTotal").val();	//贷款总额计算取值
	var L = $("#rate").val();	//贷款利率
	var x = $("#years option:selected").val();		//按揭年数取值
	var Y = x * 12;
	var R = (0.01 * L) * x / Y;			//月利率
	var pow = Math.pow((1 + R),Y);
	var top = loanTotal * R * pow;
	var down = pow - 1;
	var mavg = top/down;
	var refund = mavg * Y;		//还款总额
	var interest = refund - (loanTotal);		//支付利息款

	$("#MU").val("略");		//房贷总额
	$("#loanMU").val(loanTotal);		//贷款总额
	$("#refund").val((refund).toFixed(2));		//还款总额
	$("#interest").val((interest).toFixed(2));		//支付利息款
	$("#Down").val("0");		//首付
	$("#Y").val(Y + "(月)");	//贷款月数
	$("#mavg").val((mavg).toFixed(2));	//月均还款

};

/*贷款总额，本金计算*/
creditJin=function(){
	
	var loanTotal = $("#loanTotal").val();	//贷款总额计算取值
	var L = $("#rate").val();	//贷款利率
	var x = $("#years option:selected").val();		//按揭年数取值
	var Y = x * 12;
	var R = (0.01 * L) * x / Y;			//月利率
	var monthJin = loanTotal/Y;		//每月本金
	var totalInt = (Y + 1)*loanTotal*R/2;	//支付利息款
	var totalLl = parseInt(loanTotal)+(totalInt);	
	var month = '';
	for(var i=0; i < Y;i++){
		var monthHuan = monthJin * (i);		//累计已还本金
		var monthM = ((loanTotal/Y) + ((loanTotal - monthHuan) * R)).toFixed(2);
		month += (i + 1) + "月," + monthM + "(元)\n";
	};
	
	$("#MU").val('略');	//房款总额
	$("#loanMU").val(loanTotal);	//贷款总额
	$("#refund").val(totalLl.toFixed(2));		//还款总额
	$("#interest").val((totalInt).toFixed(2));	//支付利息款
	$("#Down").val("0");	//首期付款
	$("#Y").val(Y + "(月)");		//贷款月数
	$("#mavg1").val(month); 		//月均还款

};

});