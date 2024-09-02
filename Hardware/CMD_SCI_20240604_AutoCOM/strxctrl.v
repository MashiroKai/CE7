
/*
strxctrl #(
    .PTRWIDTH()
)
test(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.uw(uw)
    ,.wen(wen)
    ,.reply(reply)
    ,.din(din)
    ,.st(st)
    ,.valid(valid)
    ,.dout(dout)
);
*/
module strxctrl(
        input       clk
        ,input      rst_n
        ,input      [PTRWIDTH:0]uw
        ,input      wen
        ,input      reply
        ,input      [7:0]din
        ,input      [2:0]st
        ,output     reg valid
        ,output     reg [7:0]dout
);
pulsed_reg #(
    .WIDTH(1)
)
p1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.signal(reply)
    ,.pos(reply_pos)
);
parameter  PTRWIDTH = 12;
localparam DEPTH = exp2(PTRWIDTH);
localparam ST0 = 6'b000001;
localparam ST1 = 6'b000010;
localparam ST2 = 6'b000100;
localparam ST3 = 6'b001000;
localparam ST4 = 6'b010000;
localparam ST5 = 6'b100000;
reg [5:0]STATE;
reg wrsthead_cmple;
reg wrhead_asyn;
wire [2:0]wrhead_cnt;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST0;
    end
    else begin
        case (STATE)
        ST0     :begin
                if ((st == 3'b001)&&(uw<DEPTH-300)&&reply_pos) begin
                    STATE   <= ST1; 
                end
                else begin
                    STATE   <= STATE;
                end
        end
        ST1     :begin
                if (wrsthead_cmple) begin
                    STATE <= ST2;
                end
                else begin
                    STATE <= STATE;
                end
        end
        ST2     :begin
                if (!reply) begin
                    STATE <= ST0;
                end
                else begin
                    STATE <= STATE;
                end
        end
            default: STATE <= ST0;
        endcase
    end
end
always @(*) begin
    if (!rst_n) begin
        valid = 1'b0;
    end
    else begin
        case (STATE)
        ST0    : valid = 1'b0;
        ST1    : valid = 1'b1;
        ST2    : valid = wen;
            default: valid = 1'b0;
        endcase
    end
end
always @(*) begin
    if (!rst_n) begin
        dout = 8'd0;
    end
    else begin
        case (STATE)
        ST0    :begin
                dout = 8'd0;
        end 
        ST1      :begin
                case (wrhead_cnt)
                0    :dout = 8'hEB;
                1    :dout = 8'h90;
                2    :dout = 8'h00;
                3    :dout = 8'h76;
                    default: dout = 8'hEB;
                endcase
        end
        ST2     :begin
                dout = din;
        end 
            default: dout = 8'hzz;
        endcase
    end
end
always @(*) begin
    if (!rst_n) begin
        wrhead_asyn = 1'b1;
    end
    else begin
        if (STATE == ST1) begin
            wrhead_asyn = 1'b0;
        end
        else begin
            wrhead_asyn = 1'b1;
        end
    end
end
always @(*) begin
    if (!rst_n) begin
        wrsthead_cmple = 1'b0;
    end
    else begin
        if (wrhead_cnt == 3'b011) begin
            wrsthead_cmple = 1'b1;
        end
        else begin
            wrsthead_cmple = 1'b0;
        end
    end
end
counter #(
    .RST(3),
    .START(0)
)
wrsthead_cnt(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(wrhead_asyn)
    ,.en(valid)
    ,.cnt(wrhead_cnt)
);
function integer exp2 ;
input   [4:0] DEPTHWIDTH  ;
reg [4:0]temp;
begin
    exp2 = 1;
    for (temp = 0;temp <DEPTHWIDTH ;temp = temp + 1 ) begin
            exp2 = 2*exp2;
    end
end
endfunction
endmodule