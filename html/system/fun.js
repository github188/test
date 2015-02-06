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

lilv_array = new Array;
//增加数组，第一个1是贷款利率，第二个1是商业贷款，2是公积金贷款，第三个是按揭年数
//14年11月贷款利率(1.1)倍
lilv_array[1] = new Array;
lilv_array[1][1] = new Array;
lilv_array[1][2] = new Array;
lilv_array[1][1][1] = 0.0660;
lilv_array[1][1][6]	= 0.0677;
lilv_array[1][2][1] = 0.0413;
lilv_array[1][2][6] = 0.0468;
//14年11月贷款利率(85折)
lilv_array[2] = new Array;
lilv_array[2][1] = new Array;
lilv_array[2][2] = new Array;
lilv_array[2][1][1] = 0.0510;
lilv_array[2][1][6] = 0.0523;
lilv_array[2][2][1] = 0.0319;
lilv_array[2][2][6] = 0.0361;
//14年11月贷款利率(7折)
lilv_array[3] = new Array;
lilv_array[3][1] = new Array;
lilv_array[3][2] = new Array;
lilv_array[3][1][1] = 0.0420;
lilv_array[3][1][6] = 0.0431;
lilv_array[3][2][1] = 0.0263;
lilv_array[3][2][6] = 0.0297;
//14年11月贷款利率(基准)
lilv_array[4] = new Array;
lilv_array[4][1] = new Array;
lilv_array[4][2] = new Array;
lilv_array[4][1][1] = 0.0560;
lilv_array[4][1][2] = 0.0600;
lilv_array[4][1][6] = 0.0615;
lilv_array[4][2][1] = 0.0375;
lilv_array[4][2][6] = 0.0425;
//12年7月贷款利率(1.1倍)
lilv_array[5] = new Array;
lilv_array[5][1] = new Array;
lilv_array[5][2] = new Array;
lilv_array[5][1][1] = 0.0660;
lilv_array[5][1][2] = 0.0677;
lilv_array[5][1][4] = 0.0704;
lilv_array[5][1][6] = 0.0721;
lilv_array[5][2][1] = 0.0400;
lilv_array[5][2][6] = 0.0450;
//12年7月贷款利率(85折)
lilv_array[6] = new Array;
lilv_array[6][1] = new Array;
lilv_array[6][2] = new Array;
lilv_array[6][1][1] = 0.0510;
lilv_array[6][1][2] = 0.0523;
lilv_array[6][1][4] = 0.0544; 
lilv_array[6][1][6] = 0.0570;
lilv_array[6][2][1] = 0.0400;
lilv_array[6][2][6] = 0.0450;
//12年7月贷款利率(7折)
lilv_array[7] = new Array;
lilv_array[7][1] = new Array;
lilv_array[7][2] = new Array;
lilv_array[7][1][1] = 0.0420;
lilv_array[7][1][2] = 0.0431;
lilv_array[7][1][4] = 0.0448;
lilv_array[7][1][6] = 0.0459;
lilv_array[7][2][1] = 0.0400;
lilv_array[7][2][6] = 0.0450;
//12年7月贷款利率(基准)
lilv_array[8] = new Array;
lilv_array[8][1] = new Array;
lilv_array[8][2] = new Array;
lilv_array[8][1][1] = 0.0600;
lilv_array[8][1][2] = 0.0615;
lilv_array[8][1][4] = 0.0640;
lilv_array[8][1][6] = 0.0655;
lilv_array[8][2][1] = 0.0400;
lilv_array[8][2][6] = 0.0450;
//12年6月贷款利率(1.1倍)
lilv_array[9] = new Array;
lilv_array[9][1] = new Array;
lilv_array[9][2] = new Array;
lilv_array[9][1][1] = 0.0694;
lilv_array[9][1][2] = 0.0704;
lilv_array[9][1][4] = 0.0732;
lilv_array[9][1][6] = 0.0748;
lilv_array[9][2][1] = 0.0420;
lilv_array[9][2][6] = 0.0470;
//12年6月贷款利率(85折)
lilv_array[10] = new Array;
lilv_array[10][1] = new Array;
lilv_array[10][2] = new Array;
lilv_array[10][1][1] = 0.0536;
lilv_array[10][1][2] = 0.0544;
lilv_array[10][1][4] = 0.0565;
lilv_array[10][1][6] = 0.0578;
lilv_array[10][2][1] = 0.0420;
lilv_array[10][2][6] = 0.0470;
//12年6月贷款利率(7折)
lilv_array[11] = new Array;
lilv_array[11][1] = new Array;
lilv_array[11][2] = new Array;
lilv_array[11][1][1] = 0.0442;
lilv_array[11][1][2] = 0.0448;
lilv_array[11][1][4] = 0.0466;
lilv_array[11][1][6] = 0.0476;
lilv_array[11][2][1] = 0.0420;
lilv_array[11][2][6] = 0.0470;
//12年6月贷款利率(基准)
lilv_array[12] = new Array;
lilv_array[12][1] = new Array;
lilv_array[12][2] = new Array;
lilv_array[12][1][1] = 0.0631;
lilv_array[12][1][2] = 0.0640;
lilv_array[12][1][4] = 0.0665;
lilv_array[12][1][6] = 0.0680;
lilv_array[12][2][1] = 0.0420;
lilv_array[12][2][6] = 0.0470;

});