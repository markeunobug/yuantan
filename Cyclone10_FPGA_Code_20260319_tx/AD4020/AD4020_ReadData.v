module AD4020_ReadData (
	input 				sys_clk,
	input 				rst_n,
	
	//AD4020接口定义
	input 				sdo1,
	input 				sdo2,
	input 				sdo3,
	input 				sdo4,
	input 				sdo5,
	input				sdo6,
	input				sdo7,
	input				sdo8,
	input				sdo9,
	output  			sck,
	output  			dclk,
	output  			cnv,
	output 	    [7:0]	test,
	
	output 	 [19:0]  ADdata1,
	output 	 [19:0]  ADdata2,
	output 	 [19:0]  ADdata3,
	output 	 [19:0]  ADdata4,
	output 	 [19:0]  ADdata5,
	output   [19:0]  ADdata6,
	output   [19:0]  ADdata7,
	output   [19:0]  ADdata8,
	output   [19:0]  ADdata9
);


AD4020 AD4020_1(
	.sys_clk			( sys_clk),	
	.rst_n			    ( 1'b1	),
	//AD4020接口定义 
	.sdo				( sdo1	),
	.sck				( sck	),
	.dataout			( ADdata1),
	.dclk				( dclk	),
	.cnv				( cnv	),
	.test				( test )
);
AD4020 AD4020_2(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo2	),	
	.sck				( ),
	.dataout			( ADdata2),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);
AD4020 AD4020_3(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo3	),	
	.sck				( ),
	.dataout			( ADdata3),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);
AD4020 AD4020_4(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo4	),	
	.sck				( ),
	.dataout			( ADdata4),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);
AD4020 AD4020_5(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo5	),	
	.sck				( ),
	.dataout			( ADdata5),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);
AD4020 AD4020_6(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo6	),	
	.sck				( ),
	.dataout			( ADdata6),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);
AD4020 AD4020_7(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo7	),	
	.sck				( ),
	.dataout			( ADdata7),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);
AD4020 AD4020_8(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo8	),	
	.sck				( ),
	.dataout			( ADdata8),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);

AD4020 AD4020_9(
	.sys_clk			( sys_clk),
	.rst_n			    ( 1'b1	),
	.sdo				( sdo9	),	
	.sck				( ),
	.dataout			( ADdata9),
	.dclk				( ),
	.cnv				( ),
	.test				( )
);


endmodule
