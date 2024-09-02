module fifiarray (
        input       clk
        ,input      rst_n
        ,input      [4:0]cmd
        ,input      wen
        ,input      [7:0]din
        ,input      ren
        ,output     full
        ,output     rx_done
        ,output     empty
        ,output     tx_done
        ,output     valid
        ,output     [7:0]dout
);
mux1to5 mux1to5_inst
(
	.cmd(cmd) ,	// input [4:0] cmd
	.wen(wen) ,	// input  wen
	.din(din) ,	// input [7:0] din
	.full(full) ,	// output  full
	.rx_done(rx_done) ,	// output  rx_done
	.full_tc(full_tc) ,	// input  full_tc
	.rx_done_tc(rx_done_tc) ,	// input  rx_done_tc
	.valid_tc(wen_tc) ,	// output  valid_tc
	.dout_tc(din_tc) ,	// output [7:0] dout_tc
	.full_hk(full_hk) ,	// input  full_hk
	.rx_done_hk(rx_done_hk) ,	// input  rx_done_hk
	.valid_hk(wen_hk) ,	// output  valid_hk
	.dout_hk(din_hk) ,	// output [7:0] dout_hk
	.full_sd(full_sd) ,	// input  full_sd
	.rx_done_sd(rx_done_sd) ,	// input  rx_done_sd
	.valid_sd(wen_sd) ,	// output  valid_sd
	.dout_sd(din_sd) ,	// output [7:0] dout_sd
	.full_di(full_di) ,	// input  full_di
	.rx_done_di(rx_done_di) ,	// input  rx_done_di
	.valid_di(wen_di) ,	// output  valid_di
	.dout_di(din_di) ,	// output [7:0] dout_di
	.full_pf(full_pf) ,	// input  full_pf
	.rx_done_pf(rx_done_pf) ,	// input  rx_done_pf
	.valid_pf(wen_pf) ,	// output  valid_pf
	.dout_pf(din_pf) 	// output [7:0] dout_pf
);
cmdfifo #(
    .LENGTH(13)
    ,.DEPTH(256)
    ,.WIDTH(8)
)
cmdfifo_tc
(
	.clk(clk) ,	// input  clk
	.rst_n(rst_n) ,	// input  rst_n
	.wen(wen_tc) ,	// input  wen
	.din(din_tc) ,	// input [WIDTH-1:0] din
	.full(full_tc) ,	// output  full
	.ren(ren_tc) ,	// input  ren
	.valid(valid_tc) ,	// output  valid
	.dout(dout_tc) ,	// output [WIDTH-1:0] dout
	.empty(empty_tc) ,	// output  empty
	.rx_done(rx_done_tc) ,	// output  rx_done
	.tx_done(tx_done_tc) 	// output  tx_done
);
cmdfifo #(
    .LENGTH(9)
    ,.DEPTH(256)
    ,.WIDTH(8)
    
)
cmdfifo_hk
(
	.clk(clk) ,	// input  clk
	.rst_n(rst_n) ,	// input  rst_n
	.wen(wen_hk) ,	// input  wen
	.din(din_hk) ,	// input [WIDTH-1:0] din
	.full(full_hk) ,	// output  full
	.ren(ren_hk) ,	// input  ren
	.valid(valid_hk) ,	// output  valid
	.dout(dout_hk) ,	// output [WIDTH-1:0] dout
	.empty(empty_hk) ,	// output  empty
	.rx_done(rx_done_hk) ,	// output  rx_done
	.tx_done(tx_done_hk) 	// output  tx_done
);
cmdfifo #(
    .LENGTH(9)
    ,.DEPTH(256)
    ,.WIDTH(8)
    
)
cmdfifo_sd
(
	.clk(clk) ,	// input  clk
	.rst_n(rst_n) ,	// input  rst_n
	.wen(wen_sd) ,	// input  wen
	.din(din_sd) ,	// input [WIDTH-1:0] din
	.full(full_sd) ,	// output  full
	.ren(ren_sd) ,	// input  ren
	.valid(valid_sd) ,	// output  valid
	.dout(dout_sd) ,	// output [WIDTH-1:0] dout
	.empty(empty_sd) ,	// output  empty
	.rx_done(rx_done_sd) ,	// output  rx_done
	.tx_done(tx_done_sd) 	// output  tx_done
);
cmdfifo #(
    .LENGTH(25)
    ,.DEPTH(512)
    ,.WIDTH(8)
    
)
cmdfifo_di
(
	.clk(clk) ,	// input  clk
	.rst_n(rst_n) ,	// input  rst_n
	.wen(wen_di) ,	// input  wen
	.din(din_di) ,	// input [WIDTH-1:0] din
	.full(full_di) ,	// output  full
	.ren(ren_di) ,	// input  ren
	.valid(valid_di) ,	// output  valid
	.dout(dout_di) ,	// output [WIDTH-1:0] dout
	.empty(empty_di) ,	// output  empty
	.rx_done(rx_done_di) ,	// output  rx_done
	.tx_done(tx_done_di) 	// output  tx_done
);
cmdfifo #(
    .LENGTH(53)
    ,.DEPTH(512)
    ,.WIDTH(8)
    
)
cmdfifo_pf
(
	.clk(clk) ,	// input  clk
	.rst_n(rst_n) ,	// input  rst_n
	.wen(wen_pf) ,	// input  wen
	.din(din_pf) ,	// input [WIDTH-1:0] din
	.full(full_pf) ,	// output  full
	.ren(ren_pf) ,	// input  ren
	.valid(valid_pf) ,	// output  valid
	.dout(dout_pf) ,	// output [WIDTH-1:0] dout
	.empty(empty_pf) ,	// output  empty
	.rx_done(rx_done_pf) ,	// output  rx_done
	.tx_done(tx_done_pf) 	// output  tx_done
);
mux5to1 mux5to1_inst
(
	.clk(clk) ,	// input  clk
	.rst_n(rst_n) ,	// input  rst_n
	.empty_tc(empty_tc) ,	// input  empty_tc
	.tx_done_tc(tx_done_tc),	// input  tx_done_tc
	.valid_tc(valid_tc) ,	// input  valid_tc
	.din_tc(dout_tc) ,	// input [7:0] din_tc
	.ren_tc(ren_tc) ,	// output  ren_tc
	.empty_hk(empty_hk) ,	// input  empty_hk
	.tx_done_hk(tx_done_hk) ,	// input  tx_done_hk
	.valid_hk(valid_hk) ,	// input  valid_hk
	.din_hk(dout_hk) ,	// input [7:0] din_hk
	.ren_hk(ren_hk) ,	// output  ren_hk
	.empty_sd(empty_sd) ,	// input  empty_sd
	.tx_done_sd(tx_done_sd) ,	// input  tx_done_sd
	.valid_sd(valid_sd) ,	// input  valid_sd
	.din_sd(dout_sd) ,	// input [7:0] din_sd
	.ren_sd(ren_sd) ,	// output  ren_sd
	.empty_di(empty_di) ,	// input  empty_di
	.tx_done_di(tx_done_di) ,	// input  tx_done_di
	.valid_di(valid_di) ,	// input  valid_di
	.din_di(dout_di) ,	// input [7:0] din_di
	.ren_di(ren_di) ,	// output  ren_di
	.empty_pf(empty_pf) ,	// input  empty_pf
	.tx_done_pf(tx_done_pf) ,	// input  tx_done_pf
	.valid_pf(valid_pf) ,	// input  valid_pf
	.din_pf(dout_pf) ,	// input [7:0] din_pf
	.ren_pf(ren_pf) ,	// output  ren_pf
	.empty(empty) ,	// output  empty
	.tx_done(tx_done) ,	// output  tx_done
	.ren(ren) ,	// input  ren
	.valid(valid) ,	// output  valid
	.dout(dout) 	// output [7:0] dout
);


endmodule