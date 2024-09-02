/*
cmdrxctrl test(
        .clk(clk)
        ,.rst_n(rst_n)
        ,.wen(wen)
        ,.din(din)
        ,.cmd(cmd)
        ,.full(full)
        ,.newcmd(newcmd)
        ,.length(length)
        ,.rx_done(rx_done)
        ,.valid(valid)
        ,.dout(dout)
);


*/
module cmdrxctrl (
        input       clk
        ,input      rst_n
        ,input      wen
        ,input      [7:0]din
        ,input      [4:0]cmd
        ,input      full
        ,input      newcmd
        ,input      rx_done
        ,output     reg valid
        ,output     reg [7:0]dout
);
wire [2:0]wrhead_cnt;
localparam ST0 = 6'b000001;
localparam ST1 = 6'b000010;
localparam ST2 = 6'b000100;
localparam ST3 = 6'b001000;
localparam ST4 = 6'b010000;
localparam ST5 = 6'b100000;
reg [5:0]STATE;
reg wrhead_asyn;
reg wrcmdhead_cmple;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST0;
    end
    else begin
        case (STATE)
        ST0     :begin
                if (newcmd&&!full) begin
                    STATE <= ST1;
                end
                else begin
                    STATE <= STATE;
                end
        end
        ST1     :begin//HK
                if (wrcmdhead_cmple) begin
                    STATE <= ST2;
                end
                else begin
                    if (rx_done) begin
                        STATE <= ST0;
                    end
                    else begin
                        STATE <= STATE;
                    end
                end
        end
        ST2     :begin //timecode
                if (rx_done) begin
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

//wr head cnt asyn
always @(*) begin
    if (!rst_n) begin
        wrhead_asyn <= 1'b1;
    end
    else begin
        if (rx_done) begin
            wrhead_asyn <= 1'b1;
        end
        else begin
            case (STATE)
            ST0    :begin
                    wrhead_asyn <= 1'b1;
            end 
            ST1     :begin
                    wrhead_asyn <= 1'b0;
            end
            ST2     :begin
                    wrhead_asyn <= 1'b1;
            end
                default: wrhead_asyn  <= 1'b1;  
            endcase
        end
    end
end
//valid ctrl
always @(*) begin
    if (!rst_n) begin
        valid <= 1'b0;
    end
    else begin
        case (STATE)
        ST0    :    begin
                valid <= 1'b0;
        end
        ST1     :   begin
                valid <= 1'b1;  
        end
        ST2     :   begin
                valid <=wen;
        end 
            default: valid <= 1'b1;
        endcase
    end
end
//dout ctrl
always @(*) begin
    if (!rst_n) begin
        dout <= 8'd0;
    end
    else begin
    case(STATE)
    ST0:begin
        dout <= 8'd0;
    end
    ST1:begin
        case (wrhead_cnt)
        0    :dout <= 8'hEB;
        1    :dout <= 8'h90;
        2    :dout <= 8'h04;
        3    :begin
            case (cmd)
            ST0    :begin
                    dout <= 8'h13;
            end
            ST1    :begin
                    dout <= 8'h25;
            end
            ST2    :begin
                    dout <= 8'h94;
            end
            ST3    :begin
                    dout <= 8'h87;
            end
            ST4    :begin
                    dout <= 8'h63;
            end
                default: dout <= 8'h63;
            endcase
        end
            default: dout <= 8'hzz;
        endcase
    end
    ST2:begin
        dout <= din;
    end
    endcase
    end
end
counter #(
    .RST(3),
    .START(0)
)
wrcmdhead_cnt(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(wrhead_asyn)
    ,.en(valid)
    ,.cnt(wrhead_cnt)
);

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        wrcmdhead_cmple <= 1'b0;
    end
    else begin
        if (wrhead_cnt == 3'b010) begin
            wrcmdhead_cmple <= 1'b1;
        end
        else begin
            wrcmdhead_cmple <= 1'b0;;
        end
    end
end
endmodule