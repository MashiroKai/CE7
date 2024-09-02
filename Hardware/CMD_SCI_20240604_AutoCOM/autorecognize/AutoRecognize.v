module AutoRecognize(
  input        clock,
  input        reset,
  output       io_CMD_ready,
  input        io_CMD_valid,
  input  [7:0] io_CMD_bits,
  input        io_Feedback_ready,
  output       io_Feedback_valid,
  output [7:0] io_Feedback_bits
);
`ifdef RANDOMIZE_REG_INIT
  reg [31:0] _RAND_0;
  reg [31:0] _RAND_1;
  reg [31:0] _RAND_2;
  reg [31:0] _RAND_3;
  reg [31:0] _RAND_4;
`endif // RANDOMIZE_REG_INIT
  reg [31:0] CMD_Shift; // @[AutoRecognize.scala 11:24]
  reg [1:0] CNT; // @[AutoRecognize.scala 12:18]
  reg  FLAG; // @[AutoRecognize.scala 18:19]
  reg  valid; // @[AutoRecognize.scala 19:20]
  reg [7:0] out; // @[AutoRecognize.scala 20:18]
  wire [31:0] _CMD_Shift_T_1 = {io_CMD_bits,CMD_Shift[31:8]}; // @[AutoRecognize.scala 24:29]
  wire  _GEN_2 = CMD_Shift == 32'h10100cc | FLAG; // @[AutoRecognize.scala 29:24 31:13 18:19]
  wire  _T_2 = FLAG & io_Feedback_ready; // @[AutoRecognize.scala 34:21]
  wire [1:0] _CNT_T_1 = CNT - 2'h1; // @[AutoRecognize.scala 37:17]
  assign io_CMD_ready = 1'h1; // @[AutoRecognize.scala 9:14]
  assign io_Feedback_valid = valid; // @[AutoRecognize.scala 21:21]
  assign io_Feedback_bits = out; // @[AutoRecognize.scala 22:21]
  always @(posedge clock) begin
    if (reset) begin // @[AutoRecognize.scala 11:24]
      CMD_Shift <= 32'h0; // @[AutoRecognize.scala 11:24]
    end else if (CMD_Shift == 32'h10100cc) begin // @[AutoRecognize.scala 29:24]
      CMD_Shift <= 32'h0; // @[AutoRecognize.scala 30:15]
    end else if (io_CMD_valid) begin // @[AutoRecognize.scala 23:19]
      CMD_Shift <= _CMD_Shift_T_1; // @[AutoRecognize.scala 24:15]
    end
    if (reset) begin // @[AutoRecognize.scala 12:18]
      CNT <= 2'h3; // @[AutoRecognize.scala 12:18]
    end else if (FLAG & io_Feedback_ready) begin // @[AutoRecognize.scala 34:41]
      CNT <= _CNT_T_1; // @[AutoRecognize.scala 37:9]
    end
    if (reset) begin // @[AutoRecognize.scala 18:19]
      FLAG <= 1'h0; // @[AutoRecognize.scala 18:19]
    end else if (CNT == 2'h0 & io_Feedback_ready) begin // @[AutoRecognize.scala 43:37]
      FLAG <= 1'h0; // @[AutoRecognize.scala 45:10]
    end else begin
      FLAG <= _GEN_2;
    end
    if (reset) begin // @[AutoRecognize.scala 19:20]
      valid <= 1'h0; // @[AutoRecognize.scala 19:20]
    end else begin
      valid <= _T_2;
    end
    if (reset) begin // @[AutoRecognize.scala 20:18]
      out <= 8'h0; // @[AutoRecognize.scala 20:18]
    end else if (FLAG & io_Feedback_ready) begin // @[AutoRecognize.scala 34:41]
      if (2'h3 == CNT) begin // @[AutoRecognize.scala 36:12]
        out <= 8'hcc; // @[AutoRecognize.scala 36:12]
      end else if (2'h2 == CNT) begin // @[AutoRecognize.scala 36:12]
        out <= 8'h0; // @[AutoRecognize.scala 36:12]
      end else begin
        out <= 8'h10;
      end
    end else begin
      out <= 8'h0; // @[AutoRecognize.scala 40:12]
    end
  end
// Register and memory initialization
`ifdef RANDOMIZE_GARBAGE_ASSIGN
`define RANDOMIZE
`endif
`ifdef RANDOMIZE_INVALID_ASSIGN
`define RANDOMIZE
`endif
`ifdef RANDOMIZE_REG_INIT
`define RANDOMIZE
`endif
`ifdef RANDOMIZE_MEM_INIT
`define RANDOMIZE
`endif
`ifndef RANDOM
`define RANDOM $random
`endif
`ifdef RANDOMIZE_MEM_INIT
  integer initvar;
`endif
`ifndef SYNTHESIS
`ifdef FIRRTL_BEFORE_INITIAL
`FIRRTL_BEFORE_INITIAL
`endif
initial begin
  `ifdef RANDOMIZE
    `ifdef INIT_RANDOM
      `INIT_RANDOM
    `endif
    `ifndef VERILATOR
      `ifdef RANDOMIZE_DELAY
        #`RANDOMIZE_DELAY begin end
      `else
        #0.002 begin end
      `endif
    `endif
`ifdef RANDOMIZE_REG_INIT
  _RAND_0 = {1{`RANDOM}};
  CMD_Shift = _RAND_0[31:0];
  _RAND_1 = {1{`RANDOM}};
  CNT = _RAND_1[1:0];
  _RAND_2 = {1{`RANDOM}};
  FLAG = _RAND_2[0:0];
  _RAND_3 = {1{`RANDOM}};
  valid = _RAND_3[0:0];
  _RAND_4 = {1{`RANDOM}};
  out = _RAND_4[7:0];
`endif // RANDOMIZE_REG_INIT
  `endif // RANDOMIZE
end // initial
`ifdef FIRRTL_AFTER_INITIAL
`FIRRTL_AFTER_INITIAL
`endif
`endif // SYNTHESIS
endmodule
