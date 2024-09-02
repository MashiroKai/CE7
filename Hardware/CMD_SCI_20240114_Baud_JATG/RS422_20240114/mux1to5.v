
module mux1to5(
        input      [4:0]cmd
        ,input      wen
        ,input      [7:0]din
        ,output     reg full
        ,output     reg rx_done
        ,input      full_tc
        ,input      rx_done_tc
        ,output     valid_tc
        ,output     [7:0]dout_tc
        ,input      full_hk
        ,input      rx_done_hk
        ,output     valid_hk
        ,output     [7:0]dout_hk
        ,input      full_sd
        ,input      rx_done_sd
        ,output     valid_sd
        ,output     [7:0]dout_sd
        ,input      full_di
        ,input      rx_done_di
        ,output     valid_di
        ,output     [7:0]dout_di
        ,input      full_pf
        ,input      rx_done_pf
        ,output     valid_pf
        ,output     [7:0]dout_pf
);
localparam ST0 = 5'b00001;
localparam ST1 = 5'b00010;
localparam ST2 = 5'b00100;
localparam ST3 = 5'b01000;
localparam ST4 = 5'b10000;
assign  valid_tc = (cmd == ST0) ? wen:1'b0;
assign  valid_hk = (cmd == ST1) ? wen:1'b0;
assign  valid_sd = (cmd == ST2) ? wen:1'b0;
assign  valid_di = (cmd == ST3) ? wen:1'b0;
assign  valid_pf = (cmd == ST4) ? wen:1'b0;

assign  dout_tc = (cmd == ST0) ? din:8'b0;
assign  dout_hk = (cmd == ST1) ? din:8'b0;
assign  dout_sd = (cmd == ST2) ? din:8'b0;
assign  dout_di = (cmd == ST3) ? din:8'b0;
assign  dout_pf = (cmd == ST4) ? din:8'b0;

always @(*) begin
        case (cmd)
        ST0    : full = full_tc;
        ST1    : full = full_hk;
        ST2    : full = full_sd;
        ST3    : full = full_di;
        ST4    : full = full_pf;
                default: full = 1'b1;
        endcase
end
always @(*) begin
        case (cmd)
        ST0    : rx_done = rx_done_tc;
        ST1    : rx_done = rx_done_hk;
        ST2    : rx_done = rx_done_sd;
        ST3    : rx_done = rx_done_di;
        ST4    : rx_done = rx_done_pf;
                default: rx_done = 1'b0;
        endcase
end
endmodule