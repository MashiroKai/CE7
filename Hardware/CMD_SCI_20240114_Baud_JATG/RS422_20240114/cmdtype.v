module cmdtype (
        input       clk
        ,input      rst_n
        ,input      wen
        ,input      [7:0]din
        ,output     reg[4:0]cmd // platform , data injection , pre shutdown , HK,timecode 
);
reg [31:0]shift;
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            shift <= 32'b0;
        end
        else begin
            if (wen) begin
                shift <= {shift[23:0],din};
            end
            else begin
                shift <= shift;
            end
        end
end
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cmd <= 5'b00000;
        end
        else begin
                case (shift)
                {8'hEB,8'h90,8'h04,8'h63}       : begin
                        cmd <= 5'b10000;
                end
                {8'hEB,8'h90,8'h04,8'h87}       : begin
                        cmd <= 5'b01000;
                end
                {8'hEB,8'h90,8'h04,8'h94}       : begin
                        cmd <= 5'b00100;
                end
                {8'hEB,8'h90,8'h04,8'h25}       : begin
                        cmd <= 5'b00010;
                end
                {8'hEB,8'h90,8'h04,8'h13}       : begin
                        cmd <= 5'b00001;
                end
                        default: cmd <= cmd;
                endcase
        end
end
endmodule