module AD4020(
    //时钟与复位信号
    input sys_clk,
    input rst_n,
    //AD4020接口定义
    input sdo,
    output reg sck,
    output reg[19:0]dataout,
    output reg dclk,
    output reg cnv,
	 output [7:0]test
);

reg [19:0] dreceive;		//中间过程AD的暂存值
reg [7:0] current_state;
reg [7:0] next_state;

//状态码
parameter 
		Idle = 8'd0,
		Rd1=8'd1,
		Rd2=8'd2,  
		Rd3=8'd3,  
		Rd4=8'd4, 
		Rd5=8'd5,  
		Rd6=8'd6,  
		Rd7=8'd7,  
		Rd8=8'd8,	
		Rd9=8'd9,
		Rd10=8'd10,
		Rd11=8'd11,
		Rd12=8'd12,
		Rd13=8'd13,
		Rd14=8'd14,
		Rd15=8'd15,
		Rd16=8'd16,
		Rd17=8'd17,
		Rd18=8'd18,
		Rd19=8'd19,				
		Rd20=8'd20, 
		Rd21=8'd21, 
		Rd22=8'd22, 
		Rd23=8'd23, 
		Rd24=8'd24, 
		Rd25=8'd25, 
		Rd26=8'd26, 
		Rd27=8'd27, 
		Rd28=8'd28, 
		Rd29=8'd29, 
		Rd30=8'd30, 
		Rd31=8'd31, 
		Rd32=8'd32, 
		Rd33=8'd33, 
		Rd34=8'd34, 
		Rd35=8'd35, 
		Rd36=8'd36, 
		Rd37=8'd37, 
		Rd38=8'd38, 
		Rd39=8'd39, 
		Rd40=8'd40,  
		Rd41=8'd41,  
		Rd42=8'd42,  
		Rd43=8'd43,  
		Rd44=8'd44,  
		Rd45=8'd45,  
		Rd46=8'd46,  
		Rd47=8'd47,  
		Rd48=8'd48,  
		Rd49=8'd49,  
		Rd50=8'd50,  
		Rd51=8'd51,  
		Rd52=8'd52,  
		Rd53=8'd53,  
		Rd54=8'd54,  
		Rd55=8'd55,  
		Rd56=8'd56,  
		Rd57=8'd57,  
		Rd58=8'd58,  
		Rd59=8'd59,  
		Rd60=8'd60, 
		Rd61=8'd61, 
		Rd62=8'd62, 
		Rd63=8'd63,
		Rd64=8'd64,
		Rd65=8'd65,
		Rd66=8'd66,
		Rd67=8'd67,
		Rd68=8'd68,
		Rd69=8'd69,
		Rd70=8'd70, 
		Rd71=8'd71, 
		Rd72=8'd72, 
		Rd73=8'd73, 
		Rd74=8'd74, 
		Rd75=8'd75, 
		Rd76=8'd76, 
		Rd77=8'd77, 
		Rd78=8'd78, 
		RD_IDLE=8'd79;
		
//------------------------------//
//AD4020spi驱动，三段式状态机
always @(posedge sys_clk)
if(!rst_n)
	begin
	    current_state <= Idle;
	end
else
	begin
	    current_state <= next_state;		
	end

assign test = current_state[7:0];

//--------------------------------------//
always @(posedge sys_clk or negedge rst_n)
if(!rst_n)
	begin
	{dclk,sck} <= 2'b10;
	cnv <= 1'b0;
	dreceive[19:0]= 20'h00000;
	next_state = Idle;
	end
else 
	case(next_state)
	Idle:
	begin	
		{dclk,sck} <= 2'b10;
		cnv <= 1'b0;
		next_state = RD_IDLE;	
	end
	RD_IDLE: //等待cnv转换信号稳定
	begin	
		{dclk,sck} <= 2'b10;
		cnv <= 1'b0;
		next_state = Rd1;	
	end
   Rd1:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd2;		
	end		
	Rd2:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd3;		
	end	
	Rd3:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd4;		
	end		
	Rd4:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION 	
	   cnv <= 1'b1;
		next_state = Rd5;		
	end
	Rd5:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd6;		
	end		
   Rd6:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd7;		
	end		
	Rd7:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd8;		
	end	
	Rd8:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd9;		
	end		
	Rd9:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION 	
	   cnv <= 1'b1;
		next_state = Rd10;		
	end
	Rd10:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd11;		
	end		
	Rd11:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd12;		
	end			
	Rd12:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd13;		
	end		
	Rd13:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd14;		
	end	
	Rd14:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd15;		
	end		
	Rd15:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION 	
	   cnv <= 1'b1;
		next_state = Rd16;		
	end
	Rd16:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd17;		
	end		
	Rd17:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd18;		
	end		
	Rd18:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd19;		
	end	
	Rd19:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd20;		
	end		
	Rd20:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION 	
	   cnv <= 1'b1;
		next_state = Rd21;		
	end
	Rd21:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd22;		
	end		
	Rd22:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd23;		
	end		
	Rd23:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd24;		
	end	
	Rd24:	
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION
	   cnv <= 1'b1;	
		next_state = Rd25;		
	end		
	Rd25:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION 	
	   cnv <= 1'b1;
		next_state = Rd26;		
	end
	Rd26:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd27;		
	end	
	Rd27:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd28;		
	end
	Rd28:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd29;		
	end	
	Rd29:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd30;		
	end	
	Rd30:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd31;		
	end	
	Rd31:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd32;		
	end	
	Rd32:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd33;		
	end	
	Rd33:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd34;		
	end	
	Rd34:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd35;		
	end	
	Rd35:
	begin	
		{dclk,sck} <= 2'b10;//CONVERSION	
	   cnv <= 1'b1;
		next_state = Rd36;		
	end
	Rd36:
	begin	
		{dclk,sck} <= 2'b00;//tEN	
	   cnv <= 1'b0;
		next_state = Rd37;		
	end
	Rd37:
	begin	
		{dclk,sck} <= 2'b00;//tEN	
	   cnv <= 1'b0;
		next_state = Rd38;		
	end			
	Rd38:	
	begin	
		{dclk,sck} <= 2'b01;//sck low->high 
	   cnv <= 1'b0;	
		next_state = Rd39;		
	end		
	Rd39:
	begin	
		{dclk,sck} <= 2'b00;//sck high->low 	
	   cnv <= 1'b0;
		dreceive[19] <= sdo;//sdo
		next_state = Rd40;		
	end	
	Rd40:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd41;		
	end	
	Rd41:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//
		dreceive[18] <= sdo;
		next_state = Rd42;		
	end		
	Rd42:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd43;		
	end	
	Rd43:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[17] <= sdo;
		next_state = Rd44;		
	end	
	Rd44:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd45;		
	end	
	Rd45:
	begin
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[16] <= sdo;
		next_state = Rd46;		
	end	
	Rd46:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd47;		
	end	
	Rd47:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[15] <= sdo;
		next_state = Rd48;		
	end	
	Rd48:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd49;		
	end	
	Rd49:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[14] <= sdo;
		next_state = Rd50;		
	end	
	Rd50:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd51;		
	end	
	Rd51:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[13] <= sdo;
		next_state = Rd52;		
	end	
	Rd52:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd53;		
	end	
	Rd53:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[12] <= sdo;
		next_state = Rd54;		
	end	
	Rd54:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd55;		
	end	
	Rd55:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[11] <= sdo;
		next_state = Rd56;	
	end		
	Rd56:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd57;		
	end	
	Rd57:
	begin	
	   cnv <= 1'b0;
		{dclk,sck} <= 2'b00;//sck->high/
		dreceive[10] <= sdo;
		next_state = Rd58;		
	end		
	Rd58:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd59;		
	end	
	Rd59:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[9] <= sdo;
		next_state = Rd60;		
	end		
	Rd60:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd61;	
	end	
	Rd61:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[8] <= sdo;
		next_state = Rd62;		
	end		
	Rd62:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd63;		
	end	
	Rd63:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[7] <= sdo;
		next_state = Rd64;		
	end	
	Rd64:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd65;		
	end	
	Rd65:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[6] <= sdo;
		next_state = Rd66;		
	end	
	Rd66:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd67;		
	end	
	Rd67:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[5] <= sdo;
		next_state = Rd68;		
	end	
	Rd68:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd69;		
	end	
	Rd69:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[4] <= sdo;
		next_state = Rd70;		
	end	
	Rd70:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd71;		
	end	
	Rd71:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[3] <= sdo;
		next_state = Rd72;		
	end	
	Rd72:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd73;		
	end	
	Rd73:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[2] <= sdo;
		next_state = Rd74;		
	end	
	Rd74:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd75;		
	end	
	Rd75:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[1] <= sdo;
		next_state = Rd76;		
	end	
	Rd76:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b01;//sck->high				
		next_state = Rd77;		
	end	
	Rd77:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;//sck->high
		dreceive[0] <= sdo;
		next_state = Rd78;		
	end
	Rd78:
	begin
	   cnv <= 1'b0;	
		{dclk,sck} <= 2'b00;		
		next_state = Idle;		
	end			
	default: 
		begin
			cnv <= 1'b0;
			{dclk,sck} <= 2'b00;
			next_state = Idle;
		end 
	endcase
	
	
always@(posedge dclk)
	dataout <= dreceive;

endmodule
