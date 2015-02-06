$(document).ready(function(){

/*点击单选按钮*/
$('.radio').click(function() {
	$(this).removeClass('radio_no');
	$(this).addClass("radio_yes");
	$(this).siblings().removeClass("radio_yes");
	$(this).siblings().addClass("radio_no");
});

/*点击贷款总额计算显示隐藏部分*/
$('#total').click(function() {
	$('ul.ul_li > li').addClass("none");
	$("p[name='loan']").removeClass("none");
});

/*点击根据面积计算显示隐藏部分*/
$("#price").click(function() {
	$('ul.ul_li > li').removeClass("none");
	$("p[name='loan']").addClass("none");
});

/*点击等额本金显示月均金额*/
$("#jin").click(function() {
	$("#yue2").removeClass("none");
	$("#yue1").addClass("none");
});

/*点击等额本息显示月均还款*/
$("#xi").click(function() {
	$("#yue1").removeClass("none");
	$("#yue2").addClass("none");
});

});

$(document).ready(function(){

shang = $("#shang");
gong = $("#gong");
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
//计算利率值
function loanYear(z,year){
	//判断商业贷款
	if (shang.hasClass('radio_yes')) {
	
		var z = $("#loan").get(0).selectedIndex,	
			year = $("#years").get(0).selectedIndex,
			a = Math.floor(z/4),	//除 除数取整，0到2分别为14年基准、12年7月基准、12年6月基准
			b = z%4,				//取余数 0到3分别为1.1倍、85折、7折、基准
			m1 = 5.6,	//14年基准 按揭1年
			m2 = 6,		//14年基准 按揭2-5年
			m3 = 6.15,	//14年基准 按揭6-30年
			p1 = 6,		//12年7月基准 按揭1年
			p2 = 6.15,	//12年7月基准 按揭2-3年
			p3 = 6.4,	//12年7月基准 按揭4-5年
			p4 = 6.55,	//12年7月基准 按揭6-30年
			o1 = 6.31,	//12年6月基准 按揭1年
			o2 = 6.4, 	//12年6月基准 按揭2-3年
			o3 = 6.65,	//12年6月基准 按揭4-5年
			o4 = 6.8;	//12年6月基准 按揭6-30年

		if (a == 0 && b == 3 && year == 0) {
			return (m1);
		}
		else if (a == 0 && b == 3 && year > 0 && year < 5) {
			return (m2);
		}
		else if (a == 0 && b == 3 && year > 4 && year <= 22) {
			return (m3);
		}
		else if (a == 0 && b == 0 && year >= 0 && year < 5) {
			return (m2 * 1.1);
		}
		else if (a == 0 && b == 0 && year >= 5 && year <= 22 ){
			return (m3 * 1.1);
		}
		else if (a == 0 && b == 1 && year >= 0 && year < 5) {
			return (m2 * 0.85);
		}
		else if (a == 0 && b == 1 && year >= 5 && year <= 22) {
			return (m3 * 0.85);
		}
		else if (a == 0 && b == 2 && year >= 0 && year < 5) {
			return (m2 * 0.7);
		}
		else if (a == 0 && b == 2 && year >= 5 && year <= 22) {
			return (m3 * 0.7);
		}
		else if (a == 1 && b == 3 && year == 0){
			return (p1);
		}
		else if (a == 1 && b == 3 && year >= 1 && year <= 2) {
			return (p2);
		}
		else if (a == 1 && b == 3 && year >= 3 && year <= 4) {
			return (p3);
		}
		else if (a == 1 && b == 3 && year >= 5 && year <= 22) {
			return (p4);
		}
		else if (a == 1 && b == 0 && year == 0) {
			return (p1 * 1.1);
		}
		else if (a == 1 && b == 0 && year >= 1 && year <= 2) {
			return (p2 * 1.1);
		}
		else if (a == 1 && b == 0 && year >= 3 && year <= 4) {
			return (p3 * 1.1);
		}
		else if (a == 1 && b == 0 && year >= 5 && year <= 22) {
			return (p4 * 1.1);
		}
		else if (a == 1 && b == 1 && year == 0) {
			return (p1 * 0.85);
		}
		else if (a == 1 && b == 1 && year >= 1 && year <= 2) {
			return (p2 * 0.85);
		}
		else if (a == 1 && b == 1 && year >= 3 && year <= 4) {
			return (p3 * 0.85);
		}
		else if (a == 1 && b == 1 && year >= 5 && year <= 22) {
			return (p4 * 0.85);
		}
		else if (a == 1 && b == 2 && year == 0) {
			return (p1 * 0.7);
		}
		else if (a == 1 && b == 2 && year >= 1 && year <= 2) {
			return (p2 * 0.7);
		}
		else if (a == 1 && b == 2 && year >= 3 && year <= 4) {
			return (p3 * 0.7);
		}
		else if (a == 1 && b == 2 && year >= 5 && year <= 22) {
			return (p4 * 0.7);
		}
		else if (a == 2 && b == 3 && year == 0) {
			return (o1);
		}
		else if (a == 2 && b == 3 && year >= 1 && year <= 2) {
			return (o2);
		}
		else if (a == 2 && b == 3 && year >= 3 && year <= 4) {
			return (o3);
		}
		else if (a == 2 && b == 3 && year >= 5 && year <= 22) {
			return (o4);
		}
		else if (a == 2 && b == 0 && year == 0) {
			return (o1 * 1.1);
		}
		else if (a == 2 && b == 0 && year >= 1 && year <= 2) {
			return (o2 * 1.1);
		}
		else if (a == 2 && b == 0 && year >= 3 && year <= 4) {
			return (o3 * 1.1);
		}
		else if (a == 2 && b == 0 && year >= 5 && year <= 22) {
			return (o4 * 1.1);
		}
		else if (a == 2 && b == 1 && year == 0 ) {
			return (o1 * 0.85);
		}
		else if (a == 2 && b == 1 && year >= 1 && year <= 2 ) {
			return (o2 * 0.85);
		}
		else if (a == 2 && b == 1 && year >= 3 && year <= 4) {
			return (o3 * 0.85);
		}
		else if (a == 2 && b == 1 && year >= 5 && year <= 22) {
			return (o4 * 0.85);
		}
		else if (a == 2 && b == 2 && year == 0 ) {
			return (o1 * 0.75);
		}
		else if (a == 2 && b == 2 && year >= 1 && year <= 2) {
			return (o2 * 0.75);
		}
		else if (a == 2 && b == 2 && year >= 3 && year <= 4) {
			return (o3 * 0.75);
		}
		else if (a == 2 && b == 2 && year >= 5 && year <= 22) {
			return (o4 * 0.75);
		};
	}
	//判断公积金贷款
	else if (gong.hasClass('radio_yes')) {

		var z = $("#loan").get(0).selectedIndex,	
			year = $("#years").get(0).selectedIndex,	
			a = Math.floor(z/4),	//除 除数取整，0到2分别为14年基准、12年7月基准、12年6月基准
			b = z%4,				//取余数 0到3分别为1.1倍、85折、7折、基准
			m1 = 3.75,	//14年基准 按揭1-5年
			m3 = 4.25,	//14年基准 按揭6-30年
			p1 = 4,		//12年7月基准 按揭1-5年
			p4 = 4.5,	//12年7月基准 按揭6-30年
			o1 = 4.2,	//12年6月基准 按揭1-5年
			o4 = 4.7;	//12年6月基准 按揭6-30年

		if (a == 0 && b == 3 && year >= 0 && year <= 4 ) {
			return (m1);
		}
		else if (a == 0 && b == 3 && year >= 5 && year <= 22 ) {
			return (m3);
		}
		else if (a == 0 && b == 0 && year >= 0 && year <= 4 ) {
			return (m1 * 1.1);
		}
		else if (a == 0 && b == 0 && year >= 5 && year <= 22 ){
			return (m3 * 1.1);
		}
		else if (a == 0 && b == 1 && year >= 0 && year <= 4 ) {
			return (m1 * 0.85);
		}
		else if (a == 0 && b == 1 && year >= 5 && year <= 22 ) {
			return (m3 * 0.85);
		}
		else if (a == 0 && b == 2 && year >= 0 && year <= 4 ) {
			return (m1 * 0.7);
		}
		else if (a == 0 && b == 2 && year >= 5 && year <= 22) {
			return (m3 * 0.7);
		}
		else if (a == 1 && b == 3 && year >= 0 && year <= 4 ){
			return (p1);
		}
		else if (a == 1 && b == 3 && year >= 5 && year <= 22) {
			return (p4);
		}
		else if (a == 1 && b == 0 && year >= 0 && year <= 4 ) {
			return (p1);
		}
		else if (a == 1 && b == 0 && year >= 5 && year <= 22 ) {
			return (p4);
		}
		else if (a == 1 && b == 1 && year >= 0 && year <= 4 ) {
			return (p1);
		}
		else if (a == 1 && b == 1 && year >= 5 && year <= 22) {
			return (p4);
		}
		else if (a == 1 && b == 2 && year >= 0 && year <= 4 ) {
			return (p1);
		}
		else if (a == 1 && b == 2 && year >= 5 && year <= 22) {
			return (p4);
		}
		else if (a == 2 && b == 3 && year >= 0 && year <= 4 ) {
			return (o1);
		}
		else if (a == 2 && b == 3 && year >= 5 && year <= 22 ) {
			return (o4);
		}
		else if (a == 2 && b == 0 && year >= 0 && year <= 4 ) {
			return (o1);
		}
		else if (a == 2 && b == 0 && year >= 5 && year <= 22 ) {
			return (o4);
		}
		else if (a == 2 && b == 1 && year >= 0 && year <= 4 ) {
			return (o1);
		}
		else if (a == 2 && b == 1 && year >= 5 && year <= 22 ) {
			return (o4);
		}
		else if (a == 2 && b == 2 && year >= 0 && year <= 4 ) {
			return (o1);
		}
		else if (a == 2 && b == 2 && year >= 5 && year <= 22 ) {
			return (o4);
		};

	};
	
}

$("#years").on('click', function(){

	var z = $("#loan").get(0).selectedIndex,	//获取贷款利率的索引值
		y = $("#years").get(0).selectedIndex,	//获取按揭年数的索引值
		a = Math.floor(z/4),	//一共分3组
		b = z%4;	//细化到一组中的第几个

	if (a == 0 && b == 3) {
		$("#rate").val(loanYear(z,y));
	}
	else if (a == 0 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 3) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 3){
		$("#rate").val(loanYear(z,y));
	}
	else if(a == 2 && b == 0){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 1){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 2){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
});

$("#loan").on('click', function(){

	var z = $("#loan").get(0).selectedIndex,	//获取贷款利率的索引值
		y = $("#years").get(0).selectedIndex,	//获取按揭年数的索引值
		a = Math.floor(z/4),	//一共分3组
		b = z%4;	//细化到一组中的第几个

	if (a == 0 && b == 3) {
		$("#rate").val(loanYear(z,y));
	}
	else if (a == 0 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 3) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 3){
		$("#rate").val(loanYear(z,y));
	}
	else if(a == 2 && b == 0){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 1){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 2){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
});

$("#shang").on('click', function(){

	var z = $("#loan").get(0).selectedIndex,	//获取贷款利率的索引值
		y = $("#years").get(0).selectedIndex,	//获取按揭年数的索引值
		a = Math.floor(z/4),	//一共分3组
		b = z%4;	//细化到一组中的第几个

	if (a == 0 && b == 3) {
		$("#rate").val(loanYear(z,y));
	}
	else if (a == 0 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 3) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 3){
		$("#rate").val(loanYear(z,y));
	}
	else if(a == 2 && b == 0){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 1){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 2){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
});

$("#gong").on('click', function(){

	var z = $("#loan").get(0).selectedIndex,	//获取贷款利率的索引值
		y = $("#years").get(0).selectedIndex,	//获取按揭年数的索引值
		a = Math.floor(z/4),	//一共分3组
		b = z%4;	//细化到一组中的第几个

	if (a == 0 && b == 3) {
		$("#rate").val(loanYear(z,y));
	}
	else if (a == 0 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 0 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 3) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 0) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 1) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if (a == 1 && b == 2) {
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 3){
		$("#rate").val(loanYear(z,y));
	}
	else if(a == 2 && b == 0){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 1){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
	else if(a == 2 && b == 2){
		$("#rate").val(toFixeds(loanYear(z,y)));
	}
});

});

$(document).ready(function(){

//开始计算按妞
//模式切换
calc=function(){

	var shang = $("#shang");
	var gong =  $("#gong");
	var xi = $("#xi");
	var jin = $("#jin");
	var mon = $("#mon");	//单价
	var area = $("#area");	//面积
	var loanTotal = $("#loanTotal");	//贷款总额

	//贷款总额中，等额本息和等额本金
	if ($("#total").hasClass('radio_yes') && loanTotal.val() == "") {
		alert("请填写贷款总额");
		return;
	}else if ($("#total").hasClass('radio_yes') && xi.hasClass('radio_yes')) {
		creditXi();
		return;
	}else if ($("#total").hasClass('radio_yes') && jin.hasClass('radio_yes')) {
		creditJin();
		return;
	};

	//检测输入框是否填写
	if (mon.val() == "" && $("#total").hasClass('radio_no')) {
		alert("请填写单价");
		return;
	}else if (area.val() == "" && $("#total").hasClass('radio_no')) {
		alert("请填写面积");
		return;
	}else if (mon.value != '/[^1-9]/') {
		alert("请输入数字");
	};;

	//检测贷款类别和还款方式
	if (shang.hasClass('radio_yes') && xi.hasClass('radio_yes')){
		calcShang();
		return;
	}else if (gong.hasClass('radio_yes') && xi.hasClass('radio_yes')) {
		calcJin();
		return;
	}else if (shang.hasClass("radio_yes") && jin.hasClass('radio_yes')) {
		averShang();
		return;
	}else if(gong.hasClass('radio_yes') && jin.hasClass('radio_yes')){
		averJin();
		return;
	};

	//贷款总额中，等额本息和等额本金
	if ($("#total").hasClass('radio_yes') && xi.hasClass('radio_yes')) {
		alert('xi');
		creditXi();
	}else if ($("#total").hasClass('radio_yes') && jin.hasClass('radio_yes')) {
		alert('jin');
		creditJin();
	};

};

/*商业贷款计算,等额本息*/
calcShang=function(){
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

/*公积金计算,等额本息*/
calcJin=function(){

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

}

/*商业贷款计算，等额本金*/
averShang=function(){

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

/*公积金计算，等额本金*/
averJin=function(){

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