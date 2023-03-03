const std = @import("std");
const net = std.net;

pub const Server = struct {
    var buf: [1024]u8 = undefined;

    listener: *net.StreamServer,
    clients: []net.StreamServer.Connection,

    pub fn init(name: []const u8, port: u16) !Server {
        var addr = net.Address{ .in = (try net.Address.parseIp(name, port)).in };
        var lstn = net.StreamServer.init(.{ .kernel_backlog = 100, .reuse_address = true });
        try lstn.listen(addr);
        return Server{ .listener = &lstn, .clients = undefined };
    }

    pub fn start(self: Server) !void {
        while (true) {
            var conn = try self.listener.accept();
            const size = try conn.stream.read(&buf);
            _ = try conn.stream.write(buf[0..size]);
            conn.stream.close();
        }
    }

    pub fn deinit(self: Server) void {
        self.listener.deinit();
        self.listener.close();
    }
};

pub const Client = struct {
    stream: net.StreamServer.Connection,
};
