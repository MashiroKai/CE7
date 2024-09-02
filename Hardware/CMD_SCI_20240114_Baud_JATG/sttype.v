/*
sttype test(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.valid(valid)
    ,.din(din)
    ,.st(st)
    ,.newreply(newreply)
);
*/
module sttype (
        input   clk
        ,input      rst_n
        ,input      valid
        ,input      [7:0]din
        ,output     reg[2:0]st //correct , error , st
        ,output     newreply
);
reg [31:0]shift;
always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            shift <= 32'b0;
        end
        else begin
            if (valid) begin
                shift <= {shift[23:0],din};
            end
            else begin
                shift <= shift;
            end
        end
end
reg newreply_r;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        st <= 3'b000;
        newreply_r <= 1'b0;
    end
    else begin
        case (shift)
        {8'hEB,8'h90,8'h04,8'hFF}    : begin
            st <= 3'b010;
            newreply_r <= 1'b1;
        end
        {8'hEB,8'h90,8'h04,8'hF0}   : begin
            st <= 3'b100;
            newreply_r <= 1'b1;
        end
        {8'hEB,8'h90,8'h00,8'h76}   :begin
            st <= 3'b001;
            newreply_r <= 1'b1;
        end
            default: begin
            st <= st;
            newreply_r <= 1'b0;
            end
        endcase
    end
end
pulsed_reg #(
    .WIDTH(1)
)
p1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.signal(newreply_r)
    ,.pos(newreply)
);
endmodule