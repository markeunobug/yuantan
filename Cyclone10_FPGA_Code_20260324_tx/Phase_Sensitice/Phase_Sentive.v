module Phase_Sensitive(
	input 					rst_n,
	input					dclk,
	/*******正弦ROM表************/
	input signed [15:0]		cos_rom_data,	
	input signed [15:0]		sin_rom_data,
	/***AD9218数据接口***********/
	input signed [17:0] 	ADdata, 		        //AD采集结果
	/***检波后的IQ***************/
	output reg [63:0]		SIN_I,
	output reg [63:0]		COS_Q,	 
	output					Phase_Sensitive_Done  	//检波完成时钟,高电平表示完成
);

wire 		dclk_sin100;
wire [63:0] SIN_I_fir;	
wire [63:0] COS_Q_fir;
wire [33:0] result_sin;			
wire [33:0] result_cos;			 
AD_Mul_IP	MUL_sin100 (
	.clock 		( dclk 		 ), 				//dclk,这里时钟设置多大的
	.dataa 		( ADdata  ), 				    //AD数据
	.datab 		( sin_rom_data 	 ), 
	.result 	( result_sin )
);
average_25 MY_AVERAGE_SIN_6000_i (
	.rst_n	    ( rst_n	),
	.clk		( dclk	),						//时钟设置多少
	.datain		( result_sin   ),
	.dataout	( SIN_I_fir 	),
	.Done		( dclk_sin100  )
); 

AD_Mul_IP	MUL_cos100 (
	.clock 		( dclk 		 	),					//dclk.这里时钟设置多大的
	.dataa 		( ADdata  		),					//32'h0000_0001 AD数据
	.datab 		( cos_rom_data   ),
	.result 	( result_cos 		)
);
average_25 MY_AVERAGE_COS_6000_q(
	.rst_n		( rst_n	),
	.clk		( dclk	),
	.datain		( result_cos   ),
	.dataout	( COS_Q_fir 	),
	.Done		( Phase_Sensitive_Done 	)
);

//数据对其和在没有数据的时候不往RAM内写数 
always@(posedge dclk) begin
	if(Phase_Sensitive_Done && dclk_sin100) begin
		SIN_I <= SIN_I_fir;				//检波后输出的值0.5Acos相位
		COS_Q <= COS_Q_fir;				//检波后输出的值0.5Asin相位
	end
	else begin
		//	SIN_I <= SIN_I;
		//	COS_Q <= COS_Q;
			SIN_I <= 64'bzzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz;//检波后输出的值0.5Acos相位
			COS_Q <= 64'bzzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz;//检波后输出的值0.5Asin相位
	end
end

endmodule

