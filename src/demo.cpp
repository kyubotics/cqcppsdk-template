#include <iostream>
#include <set>
#include <sstream>

#include <cqcppsdk/cqcppsdk.h>

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;

CQ_INIT {
    on_enable([] { logging::info("启用", "插件已启用"); });

    on_private_message([](const PrivateMessageEvent &e) {
        try {
            auto msgid = send_private_message(e.user_id, e.message); // 直接复读消息
            logging::info_success("私聊", "私聊消息复读完成, 消息 Id: " + to_string(msgid));
            send_message(e.target,
                         MessageSegment::face(111) + "这是通过 message 模块构造的消息~"); // 使用 message 模块构造消息
        } catch (ApiError &e) {
            logging::warning("私聊", "私聊消息复读失败, 错误码: " + to_string(e.code));
        }
    });

    on_message([](const MessageEvent &e) {
        logging::debug("消息", "收到消息: " + e.message + "\n实际类型: " + typeid(e).name());
    });

    on_group_message([](const GroupMessageEvent &e) {
        static const set<int64_t> ENABLED_GROUPS = {123456, 123457};
        if (ENABLED_GROUPS.count(e.group_id) == 0) return; // 不在启用的群中, 忽略

        try {
            send_message(e.target, e.message); // 复读
            auto mem_list = get_group_member_list(e.group_id); // 获取群成员列表
            string msg;
            for (auto i = 0; i < min(10, static_cast<int>(mem_list.size())); i++) {
                msg += "昵称: " + mem_list[i].nickname + "\n"; // 拼接前十个成员的昵称
            }
            send_group_message(e.group_id, msg); // 发送群消息
        } catch (ApiError &) { // 忽略发送失败
        }
        if (e.is_anonymous()) {
            logging::info("群聊", "消息是匿名消息, 匿名昵称: " + e.anonymous.name);
        }
        e.block(); // 阻止当前事件传递到下一个插件
    });

    on_group_upload([](const auto &e) { // 可以使用 auto 自动推断类型
        stringstream ss;
        ss << "您上传了一个文件, 文件名: " << e.file.name << ", 大小(字节): " << e.file.size;
        try {
            send_message(e.target, ss.str());
        } catch (ApiError &) {
        }
    });
}

CQ_MENU(menu_demo_1) {
    logging::info("菜单", "点击菜单1");
}

CQ_MENU(menu_demo_2) {
    send_private_message(10000, "测试");
}
