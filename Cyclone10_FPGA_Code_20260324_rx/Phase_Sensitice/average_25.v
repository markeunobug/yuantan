module average_25(
	input 					rst_n,
	input 					clk,
	input  signed [35:0]	datain,
	output signed [63:0]	dataout,
	output reg				Done
);

//------------------------------//
reg [15:0] cnt;
reg signed [63:0] sum;
reg signed [63:0] sum_r;

always @(posedge clk) begin
	if(!rst_n)
		begin
			sum <= 64'd0;
			cnt <= 16'd0;
			Done <= 1'b0;	
		end
	else if(cnt < 16'd500) begin
			cnt <= cnt + 1'b1;
			sum <= sum + datain;
			Done <= 1'b0;	
	end
	else if(cnt < 16'd3000) begin
			cnt <= cnt + 1'b1;
			sum <= sum + datain;
			Done <= 1'b1;	
	end
	else if(cnt < 16'd6000)	begin
			cnt <= cnt + 1'b1;
			sum <= sum + datain;
			Done <= 1'b0;	
	end
	else begin
			sum_r <= sum;
			cnt <= 16'd0;
			sum <= 64'd0;
			Done <= 1'b0;
	end
end

assign 	dataout =  sum_r;

endmodule

