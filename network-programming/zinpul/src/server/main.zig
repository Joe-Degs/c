//! zinpul chat server
const std = @import("std");
const zinlib = @import("zinlib");

pub fn main() !void {
    var server = try zinlib.Net.Server.init("127.0.0.1", 2022);
    defer server.deinit();
    try server.start();
}
