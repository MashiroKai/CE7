module mux8(
	input	[7:0]din,
	input valid,
	output	[7:0]dout
);
 assign dout= valid?din:8'b0;
endmodule