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

$(document).ready(function() {

shang = $("#shang");
gong = $("#gong");

/*改变按揭年数，#rate 变化*/
$("#years").on('click', function(){

	var yearName = $("#years option:selected").html(),
		loanName = $("#loan option:selected").html();

	if (shang.hasClass('radio_yes')) {
		
		if (loanName == "14年11月22日利率上限(1.1倍)") {
			
				$("#rate").val("6.77");

				$("#years").ready(function(){

					switch (yearName){
						case '1年 (12期)':
							$("#rate").val("6.6");
							break;
						case '2年 (24期)':
							$("#rate").val("6.6");
							break;
						case '3年 (36期)':
							$("#rate").val("6.6");
							break;
						case '4年 (48期)':
							$("#rate").val("6.6");
							break;
						case '5年 (60期)':
							$("#rate").val("6.6");
							break;
						default:
							$("#rate").val("6.77");
					}
				return yearName;
				});
		}
		else if (loanName == "14年11月22日利率下限(85折)") {

			$("#rate").val("5.23");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.1");
						break;
					case '2年 (24期)':
						$("#rate").val("5.1");
						break;
					case '3年 (36期)':
						$("#rate").val("5.1");
						break;
					case '4年 (48期)':
						$("#rate").val("5.1");
						break;
					case '5年 (60期)':
						$("#rate").val("5.1");
						break;
					default:
						$("#rate").val("5.23");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(7折)") {

			$("#rate").val("4.31");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.31");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日基准利率") {

			$("#rate").val("6.15");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6");
						break;
					case '3年 (36期)':
						$("#rate").val("6");
						break;
					case '4年 (48期)':
						$("#rate").val("6");
						break;
					case '5年 (60期)':
						$("#rate").val("6");
						break;
					default:
						$("#rate").val("6.15");
				}
			return yearName;
			});
		}
		else if (loanName == "12年7月6日利率上限(1.1倍)") {

			$("#rate").val("7.21");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.77");
						break;
					case '3年 (36期)':
						$("#rate").val("6.77");
						break;
					case '4年 (48期)':
						$("#rate").val("7.04");
						break;
					case '5年 (60期)':
						$("#rate").val("7.04");
						break;
					default:
						$("#rate").val("7.21");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(85折)") {

			$("#rate").val("5.57");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.1");
						break;
					case '2年 (24期)':
						$("#rate").val("5.23");
						break;
					case '3年 (36期)':
						$("#rate").val("5.23");
						break;
					case '4年 (48期)':
						$("#rate").val("5.44");
						break;
					case '5年 (60期)':
						$("#rate").val("5.44");
						break;
					default:
						$("#rate").val("5.57");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(7折)") {

			$("#rate").val("4.59");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.31");
						break;
					case '3年 (36期)':
						$("#rate").val("4.31");
						break;
					case '4年 (48期)':
						$("#rate").val("4.48");
						break;
					case '5年 (60期)':
						$("#rate").val("4.48");
						break;
					default:
						$("#rate").val("4.59");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日基准利率") {

			$("#rate").val("6.55");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.15");
						break;
					case '3年 (36期)':
						$("#rate").val("6.15");
						break;
					case '4年 (48期)':
						$("#rate").val("6.4");
						break;
					case '5年 (60期)':
						$("#rate").val("6.4");
						break;
					default:
						$("#rate").val("6.55");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率上限(1.1倍)") {

			$("#rate").val("7.48");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.94");
						break;
					case '2年 (24期)':
						$("#rate").val("7.04");
						break;
					case '3年 (36期)':
						$("#rate").val("7.04");
						break;
					case '4年 (48期)':
						$("#rate").val("7.32");
						break;
					case '5年 (60期)':
						$("#rate").val("7.32");
						break;
					default:
						$("#rate").val("7.48");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(85折)") {

			$("#rate").val("5.78");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.36");
						break;
					case '2年 (24期)':
						$("#rate").val("5.44");
						break;
					case '3年 (36期)':
						$("#rate").val("5.44");
						break;
					case '4年 (48期)':
						$("#rate").val("5.65");
						break;
					case '5年 (60期)':
						$("#rate").val("5.65");
						break;
					default:
						$("#rate").val("5.78");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(7折)") {

			$("#rate").val("4.76");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.42");
						break;
					case '2年 (24期)':
						$("#rate").val("4.48");
						break;
					case '3年 (36期)':
						$("#rate").val("4.48");
						break;
					case '4年 (48期)':
						$("#rate").val("4.66");
						break;
					case '5年 (60期)':
						$("#rate").val("4.66");
						break;
					default:
						$("#rate").val("4.76");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日基准利率") {

			$("#rate").val("6.8");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.31");
						break;
					case '2年 (24期)':
						$("#rate").val("6.4");
						break;
					case '3年 (36期)':
						$("#rate").val("6.4");
						break;
					case '4年 (48期)':
						$("#rate").val("6.65");
						break;
					case '5年 (60期)':
						$("#rate").val("6.65");
						break;
					default:
						$("#rate").val("6.8");
					}
				return yearName;
			});
		}

	}
	else if (gong.hasClass('radio_yes')) {

		if (loanName == "14年11月22日利率上限(1.1倍)") {
			
			$("#rate").val("4.68");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.13");
						break;
					case '2年 (24期)':
						$("#rate").val("4.13");
						break;
					case '3年 (36期)':
						$("#rate").val("4.13");
						break;
					case '4年 (48期)':
						$("#rate").val("4.13");
						break;
					case '5年 (60期)':
						$("#rate").val("4.13");
						break;
					default:
						$("#rate").val("4.68");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(85折)") {

			$("#rate").val("3.61");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("3.19");
						break;
					case '2年 (24期)':
						$("#rate").val("3.19");
						break;
					case '3年 (36期)':
						$("#rate").val("3.19");
						break;
					case '4年 (48期)':
						$("#rate").val("3.19");
						break;
					case '5年 (60期)':
						$("#rate").val("3.19");
						break;
					default:
						$("#rate").val("3.61");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(7折)") {

			$("#rate").val("2.97");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("2.63");
						break;
					case '2年 (24期)':
						$("#rate").val("2.63");
						break;
					case '3年 (36期)':
						$("#rate").val("2.63");
						break;
					case '4年 (48期)':
						$("#rate").val("2.63");
						break;
					case '5年 (60期)':
						$("#rate").val("2.63");
						break;
					default:
						$("#rate").val("2.97");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日基准利率") {

			$("#rate").val("4.25");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("3.75");
						break;
					case '2年 (24期)':
						$("#rate").val("3.75");
						break;
					case '3年 (36期)':
						$("#rate").val("3.75");
						break;
					case '4年 (48期)':
						$("#rate").val("3.75");
						break;
					case '5年 (60期)':
						$("#rate").val("3.75");
						break;
					default:
						$("#rate").val("4.25");
				}
			return yearName;
			});
		}
		else if (loanName == "12年7月6日利率上限(1.1倍)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(85折)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(7折)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日基准利率") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率上限(1.1倍)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(85折)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(7折)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日基准利率") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}

	};
});

/*判断默认贷款利率的情况下，改变按揭年数， #rate 变化*/
$("#loan").on('click', function(){
	
	var	loanName = $("#loan option:selected").html(),
		yearName = $("#years option:selected").html();

	if (shang.hasClass('radio_yes')) {

		if (loanName == "14年11月22日利率上限(1.1倍)") {
			
				$("#rate").val("6.77");

				$("#years").ready(function(){

					switch (yearName){
						case '1年 (12期)':
							$("#rate").val("6.6");
							break;
						case '2年 (24期)':
							$("#rate").val("6.6");
							break;
						case '3年 (36期)':
							$("#rate").val("6.6");
							break;
						case '4年 (48期)':
							$("#rate").val("6.6");
							break;
						case '5年 (60期)':
							$("#rate").val("6.6");
							break;
						default:
							$("#rate").val("6.77");
					}
				return yearName;
				});
		}
		else if (loanName == "14年11月22日利率下限(85折)") {

			$("#rate").val("5.23");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.1");
						break;
					case '2年 (24期)':
						$("#rate").val("5.1");
						break;
					case '3年 (36期)':
						$("#rate").val("5.1");
						break;
					case '4年 (48期)':
						$("#rate").val("5.1");
						break;
					case '5年 (60期)':
						$("#rate").val("5.1");
						break;
					default:
						$("#rate").val("5.23");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(7折)") {

			$("#rate").val("4.31");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.31");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日基准利率") {

			$("#rate").val("6.15");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6");
						break;
					case '3年 (36期)':
						$("#rate").val("6");
						break;
					case '4年 (48期)':
						$("#rate").val("6");
						break;
					case '5年 (60期)':
						$("#rate").val("6");
						break;
					default:
						$("#rate").val("6.15");
				}
			return yearName;
			});
		}
		else if (loanName == "12年7月6日利率上限(1.1倍)") {

			$("#rate").val("7.21");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.77");
						break;
					case '3年 (36期)':
						$("#rate").val("6.77");
						break;
					case '4年 (48期)':
						$("#rate").val("7.04");
						break;
					case '5年 (60期)':
						$("#rate").val("7.04");
						break;
					default:
						$("#rate").val("7.21");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(85折)") {

			$("#rate").val("5.57");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.1");
						break;
					case '2年 (24期)':
						$("#rate").val("5.23");
						break;
					case '3年 (36期)':
						$("#rate").val("5.23");
						break;
					case '4年 (48期)':
						$("#rate").val("5.44");
						break;
					case '5年 (60期)':
						$("#rate").val("5.44");
						break;
					default:
						$("#rate").val("5.57");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(7折)") {

			$("#rate").val("4.59");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.31");
						break;
					case '3年 (36期)':
						$("#rate").val("4.31");
						break;
					case '4年 (48期)':
						$("#rate").val("4.48");
						break;
					case '5年 (60期)':
						$("#rate").val("4.48");
						break;
					default:
						$("#rate").val("4.59");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日基准利率") {

			$("#rate").val("6.55");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.15");
						break;
					case '3年 (36期)':
						$("#rate").val("6.15");
						break;
					case '4年 (48期)':
						$("#rate").val("6.4");
						break;
					case '5年 (60期)':
						$("#rate").val("6.4");
						break;
					default:
						$("#rate").val("6.55");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率上限(1.1倍)") {

			$("#rate").val("7.48");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.94");
						break;
					case '2年 (24期)':
						$("#rate").val("7.04");
						break;
					case '3年 (36期)':
						$("#rate").val("7.04");
						break;
					case '4年 (48期)':
						$("#rate").val("7.32");
						break;
					case '5年 (60期)':
						$("#rate").val("7.32");
						break;
					default:
						$("#rate").val("7.48");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(85折)") {

			$("#rate").val("5.78");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.36");
						break;
					case '2年 (24期)':
						$("#rate").val("5.44");
						break;
					case '3年 (36期)':
						$("#rate").val("5.44");
						break;
					case '4年 (48期)':
						$("#rate").val("5.65");
						break;
					case '5年 (60期)':
						$("#rate").val("5.65");
						break;
					default:
						$("#rate").val("5.78");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(7折)") {

			$("#rate").val("4.76");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.42");
						break;
					case '2年 (24期)':
						$("#rate").val("4.48");
						break;
					case '3年 (36期)':
						$("#rate").val("4.48");
						break;
					case '4年 (48期)':
						$("#rate").val("4.66");
						break;
					case '5年 (60期)':
						$("#rate").val("4.66");
						break;
					default:
						$("#rate").val("4.76");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日基准利率") {

			$("#rate").val("6.8");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.31");
						break;
					case '2年 (24期)':
						$("#rate").val("6.4");
						break;
					case '3年 (36期)':
						$("#rate").val("6.4");
						break;
					case '4年 (48期)':
						$("#rate").val("6.65");
						break;
					case '5年 (60期)':
						$("#rate").val("6.65");
						break;
					default:
						$("#rate").val("6.8");
					}
				return yearName;
			});
		}
	}
	else if (gong.hasClass('radio_yes')) {
		
		if (loanName == "14年11月22日利率上限(1.1倍)") {
			
			$("#rate").val("4.68");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.13");
						break;
					case '2年 (24期)':
						$("#rate").val("4.13");
						break;
					case '3年 (36期)':
						$("#rate").val("4.13");
						break;
					case '4年 (48期)':
						$("#rate").val("4.13");
						break;
					case '5年 (60期)':
						$("#rate").val("4.13");
						break;
					default:
						$("#rate").val("4.68");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(85折)") {

			$("#rate").val("3.61");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("3.19");
						break;
					case '2年 (24期)':
						$("#rate").val("3.19");
						break;
					case '3年 (36期)':
						$("#rate").val("3.19");
						break;
					case '4年 (48期)':
						$("#rate").val("3.19");
						break;
					case '5年 (60期)':
						$("#rate").val("3.19");
						break;
					default:
						$("#rate").val("3.61");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(7折)") {

			$("#rate").val("2.97");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("2.63");
						break;
					case '2年 (24期)':
						$("#rate").val("2.63");
						break;
					case '3年 (36期)':
						$("#rate").val("2.63");
						break;
					case '4年 (48期)':
						$("#rate").val("2.63");
						break;
					case '5年 (60期)':
						$("#rate").val("2.63");
						break;
					default:
						$("#rate").val("2.97");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日基准利率") {

			$("#rate").val("4.25");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("3.75");
						break;
					case '2年 (24期)':
						$("#rate").val("3.75");
						break;
					case '3年 (36期)':
						$("#rate").val("3.75");
						break;
					case '4年 (48期)':
						$("#rate").val("3.75");
						break;
					case '5年 (60期)':
						$("#rate").val("3.75");
						break;
					default:
						$("#rate").val("4.25");
				}
			return yearName;
			});
		}
		else if (loanName == "12年7月6日利率上限(1.1倍)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(85折)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(7折)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日基准利率") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率上限(1.1倍)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(85折)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(7折)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日基准利率") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
	};
});

/*改变商业贷款模式下的按揭年数和贷款利率各项，#rate 变化 */
$("#shang").on('click', function(){
	
	/*$("#rate").val("6.15");*/

	$("#loan").ready(function(){
		
		var	loanName = $("#loan option:selected").html(),
			yearName = $("#years option:selected").html();

		if (loanName == "14年11月22日利率上限(1.1倍)") {
			
			$("#rate").val("6.77");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.6");
						break;
					case '3年 (36期)':
						$("#rate").val("6.6");
						break;
					case '4年 (48期)':
						$("#rate").val("6.6");
						break;
					case '5年 (60期)':
						$("#rate").val("6.6");
						break;
					default:
						$("#rate").val("6.77");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(85折)") {

			$("#rate").val("5.23");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.1");
						break;
					case '2年 (24期)':
						$("#rate").val("5.1");
						break;
					case '3年 (36期)':
						$("#rate").val("5.1");
						break;
					case '4年 (48期)':
						$("#rate").val("5.1");
						break;
					case '5年 (60期)':
						$("#rate").val("5.1");
						break;
					default:
						$("#rate").val("5.23");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(7折)") {

			$("#rate").val("4.31");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.31");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日基准利率") {

			$("#rate").val("6.15");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6");
						break;
					case '3年 (36期)':
						$("#rate").val("6");
						break;
					case '4年 (48期)':
						$("#rate").val("6");
						break;
					case '5年 (60期)':
						$("#rate").val("6");
						break;
					default:
						$("#rate").val("6.15");
				}
			return yearName;
			});
		}
		else if (loanName == "12年7月6日利率上限(1.1倍)") {

			$("#rate").val("7.21");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.77");
						break;
					case '3年 (36期)':
						$("#rate").val("6.77");
						break;
					case '4年 (48期)':
						$("#rate").val("7.04");
						break;
					case '5年 (60期)':
						$("#rate").val("7.04");
						break;
					default:
						$("#rate").val("7.21");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(85折)") {

			$("#rate").val("5.57");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.1");
						break;
					case '2年 (24期)':
						$("#rate").val("5.23");
						break;
					case '3年 (36期)':
						$("#rate").val("5.23");
						break;
					case '4年 (48期)':
						$("#rate").val("5.44");
						break;
					case '5年 (60期)':
						$("#rate").val("5.44");
						break;
					default:
						$("#rate").val("5.57");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(7折)") {

			$("#rate").val("4.59");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.31");
						break;
					case '3年 (36期)':
						$("#rate").val("4.31");
						break;
					case '4年 (48期)':
						$("#rate").val("4.48");
						break;
					case '5年 (60期)':
						$("#rate").val("4.48");
						break;
					default:
						$("#rate").val("4.59");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日基准利率") {

			$("#rate").val("6.55");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6");
						break;
					case '2年 (24期)':
						$("#rate").val("6.15");
						break;
					case '3年 (36期)':
						$("#rate").val("6.15");
						break;
					case '4年 (48期)':
						$("#rate").val("6.4");
						break;
					case '5年 (60期)':
						$("#rate").val("6.4");
						break;
					default:
						$("#rate").val("6.55");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率上限(1.1倍)") {

			$("#rate").val("7.48");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.94");
						break;
					case '2年 (24期)':
						$("#rate").val("7.04");
						break;
					case '3年 (36期)':
						$("#rate").val("7.04");
						break;
					case '4年 (48期)':
						$("#rate").val("7.32");
						break;
					case '5年 (60期)':
						$("#rate").val("7.32");
						break;
					default:
						$("#rate").val("7.48");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(85折)") {

			$("#rate").val("5.78");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("5.36");
						break;
					case '2年 (24期)':
						$("#rate").val("5.44");
						break;
					case '3年 (36期)':
						$("#rate").val("5.44");
						break;
					case '4年 (48期)':
						$("#rate").val("5.65");
						break;
					case '5年 (60期)':
						$("#rate").val("5.65");
						break;
					default:
						$("#rate").val("5.78");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(7折)") {

			$("#rate").val("4.76");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.42");
						break;
					case '2年 (24期)':
						$("#rate").val("4.48");
						break;
					case '3年 (36期)':
						$("#rate").val("4.48");
						break;
					case '4年 (48期)':
						$("#rate").val("4.66");
						break;
					case '5年 (60期)':
						$("#rate").val("4.66");
						break;
					default:
						$("#rate").val("4.76");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日基准利率") {

			$("#rate").val("6.8");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("6.31");
						break;
					case '2年 (24期)':
						$("#rate").val("6.4");
						break;
					case '3年 (36期)':
						$("#rate").val("6.4");
						break;
					case '4年 (48期)':
						$("#rate").val("6.65");
						break;
					case '5年 (60期)':
						$("#rate").val("6.65");
						break;
					default:
						$("#rate").val("6.8");
					}
				return yearName;
			});
		}
	});
});

/*改变公积金贷款模式下的按揭年数和贷款利率各项， #rate 变化*/
$("#gong").on('click', function(){

	/*$("#rate").val("4.25");*/

	$("#loan").ready(function(){
		
		var	loanName = $("#loan option:selected").html(),
			yearName = $("#years option:selected").html();

		if (loanName == "14年11月22日利率上限(1.1倍)") {
			
			$("#rate").val("4.68");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.13");
						break;
					case '2年 (24期)':
						$("#rate").val("4.13");
						break;
					case '3年 (36期)':
						$("#rate").val("4.13");
						break;
					case '4年 (48期)':
						$("#rate").val("4.13");
						break;
					case '5年 (60期)':
						$("#rate").val("4.13");
						break;
					default:
						$("#rate").val("4.68");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(85折)") {

			$("#rate").val("3.61");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("3.19");
						break;
					case '2年 (24期)':
						$("#rate").val("3.19");
						break;
					case '3年 (36期)':
						$("#rate").val("3.19");
						break;
					case '4年 (48期)':
						$("#rate").val("3.19");
						break;
					case '5年 (60期)':
						$("#rate").val("3.19");
						break;
					default:
						$("#rate").val("3.61");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日利率下限(7折)") {

			$("#rate").val("2.97");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("2.63");
						break;
					case '2年 (24期)':
						$("#rate").val("2.63");
						break;
					case '3年 (36期)':
						$("#rate").val("2.63");
						break;
					case '4年 (48期)':
						$("#rate").val("2.63");
						break;
					case '5年 (60期)':
						$("#rate").val("2.63");
						break;
					default:
						$("#rate").val("2.97");
				}
			return yearName;
			});
		}
		else if (loanName == "14年11月22日基准利率") {

			$("#rate").val("4.25");
			
			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("3.75");
						break;
					case '2年 (24期)':
						$("#rate").val("3.75");
						break;
					case '3年 (36期)':
						$("#rate").val("3.75");
						break;
					case '4年 (48期)':
						$("#rate").val("3.75");
						break;
					case '5年 (60期)':
						$("#rate").val("3.75");
						break;
					default:
						$("#rate").val("4.25");
				}
			return yearName;
			});
		}
		else if (loanName == "12年7月6日利率上限(1.1倍)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(85折)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日利率下限(7折)") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年7月6日基准利率") {

			$("#rate").val("4.5");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4");
						break;
					case '2年 (24期)':
						$("#rate").val("4");
						break;
					case '3年 (36期)':
						$("#rate").val("4");
						break;
					case '4年 (48期)':
						$("#rate").val("4");
						break;
					case '5年 (60期)':
						$("#rate").val("4");
						break;
					default:
						$("#rate").val("4.5");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率上限(1.1倍)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(85折)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日利率下限(7折)") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
		else if (loanName == "12年6月8日基准利率") {

			$("#rate").val("4.7");

			$("#years").ready(function(){

				switch (yearName){
					case '1年 (12期)':
						$("#rate").val("4.2");
						break;
					case '2年 (24期)':
						$("#rate").val("4.2");
						break;
					case '3年 (36期)':
						$("#rate").val("4.2");
						break;
					case '4年 (48期)':
						$("#rate").val("4.2");
						break;
					case '5年 (60期)':
						$("#rate").val("4.2");
						break;
					default:
						$("#rate").val("4.7");
					}
				return yearName;
			});
		}
	});
});

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
	};

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


/*	var loan = $("#loan > option:selected").val();
	$("#loan").change(function(){
		$("#rate").val($("#loan").val());
	});*/

	