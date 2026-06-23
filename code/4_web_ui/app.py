# -*- coding: utf-8 -*-
"""
HE THONG QUAN TRI LINUX - Giao dien web cho 3 phan dau cua de tai
(Lap trinh shell, lap trinh C, module nhan).

Backend Flask thuc thi cac thao tac he thong that su tren Ubuntu:
  - Phan 1: quan ly tep, lap lich crontab, thoi gian he thong, goi phan mem
  - Phan 2: tien trinh, tep (C), thong tin mang, socket TCP (client-server)
  - Phan 3: bien dich / nap / go module nhan, xem dmesg

Chay:  python3 app.py    (mac dinh http://127.0.0.1:5000)
Luu y: cac thao tac doi gio / cai-go goi / module nhan can quyen sudo.
"""
import os
import shutil
import socket
import threading
import subprocess
from datetime import datetime
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

# Thu muc lam viec an toan cho chuc nang quan ly tep
WORKSPACE = os.path.expanduser('~/ltnln_workspace')
os.makedirs(WORKSPACE, exist_ok=True)

# Thu muc chua ma nguon module nhan (Phan 3)
KERNEL_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '3_kernel'))

# Cong cho socket demo (Phan 2)
SOCKET_PORT = 9099


# ------------------------------------------------------------------ helpers
def run(cmd, cwd=None, timeout=60):
    """Chay mot lenh he thong, tra ve (ok, output)."""
    try:
        p = subprocess.run(cmd, cwd=cwd, timeout=timeout,
                           capture_output=True, text=True)
        out = (p.stdout or '') + (p.stderr or '')
        return p.returncode == 0, out.strip() or '(khong co dau ra)'
    except FileNotFoundError:
        return False, 'Loi: khong tim thay lenh "%s" tren he thong.' % (
            cmd[0] if isinstance(cmd, list) else cmd)
    except subprocess.TimeoutExpired:
        return False, 'Loi: lenh chay qua thoi gian cho phep.'
    except Exception as e:                              # noqa
        return False, 'Loi: %s' % e


def safe_path(p):
    """Gioi han thao tac trong WORKSPACE de an toan."""
    if not p:
        return WORKSPACE
    full = os.path.abspath(os.path.join(WORKSPACE, p))
    if not full.startswith(WORKSPACE):
        return WORKSPACE
    return full


def J(ok, output):
    return jsonify(ok=bool(ok), output=output)


# ================================================================== TRANG
SECTIONS = [
    ('Trang chủ', 'index', None),
]


@app.route('/')
def index():
    return render_template('index.html', active='home')


@app.route('/p1/file')
def p1_file():
    return render_template('file.html', active='p1_file', title='Quản lý tệp tin',
                           part='PHẦN 1 · LẬP TRÌNH SHELL', csource='quanly_file.sh')


@app.route('/p1/cron')
def p1_cron():
    return render_template('cron.html', active='p1_cron', title='Lập lịch tác vụ',
                           part='PHẦN 1 · LẬP TRÌNH SHELL')


@app.route('/p1/time')
def p1_time():
    return render_template('time.html', active='p1_time', title='Thời gian hệ thống',
                           part='PHẦN 1 · LẬP TRÌNH SHELL')


@app.route('/p1/pkg')
def p1_pkg():
    return render_template('pkg.html', active='p1_pkg', title='Quản lý gói phần mềm',
                           part='PHẦN 1 · LẬP TRÌNH SHELL')


@app.route('/p2/proc')
def p2_proc():
    return render_template('proc.html', active='p2_proc', title='Quản lý tiến trình',
                           part='PHẦN 2 · LẬP TRÌNH C')


@app.route('/p2/file')
def p2_file():
    return render_template('file.html', active='p2_file', title='Quản lý tệp tin (C)',
                           part='PHẦN 2 · LẬP TRÌNH C', csource='quanly_file.c')


@app.route('/p2/net')
def p2_net():
    return render_template('net.html', active='p2_net', title='Thông tin mạng',
                           part='PHẦN 2 · LẬP TRÌNH C')


@app.route('/p2/socket')
def p2_socket():
    return render_template('socket.html', active='p2_socket', title='Giao tiếp Socket TCP',
                           part='PHẦN 2 · LẬP TRÌNH C', port=SOCKET_PORT)


@app.route('/p3/kernel')
def p3_kernel():
    return render_template('kernel.html', active='p3_kernel', title='Module nhân Linux',
                           part='PHẦN 3 · LẬP TRÌNH NHÂN')


# ================================================================ API PHAN 1
# ---- Quan ly tep ----
@app.route('/api/file/list', methods=['POST'])
def api_file_list():
    rel = request.form.get('path', '')
    base = safe_path(rel)
    if not os.path.isdir(base):
        return J(False, 'Thư mục không tồn tại.')
    rows = []
    try:
        for name in sorted(os.listdir(base)):
            fp = os.path.join(base, name)
            st = os.stat(fp)
            rows.append({
                'name': name,
                'type': 'Thư mục' if os.path.isdir(fp) else 'Tệp tin',
                'size': st.st_size,
                'perm': oct(st.st_mode)[-3:],
                'mtime': datetime.fromtimestamp(st.st_mtime).strftime('%d/%m/%Y %H:%M'),
            })
    except Exception as e:                              # noqa
        return J(False, 'Lỗi: %s' % e)
    relshow = os.path.relpath(base, WORKSPACE)
    return jsonify(ok=True, cwd='/' if relshow == '.' else '/' + relshow, rows=rows)


@app.route('/api/file/op', methods=['POST'])
def api_file_op():
    op = request.form.get('op')
    a = request.form.get('a', '')
    b = request.form.get('b', '')
    pa = safe_path(a)
    try:
        if op == 'mkdir':
            os.makedirs(pa, exist_ok=True);  msg = 'Đã tạo thư mục: %s' % a
        elif op == 'touch':
            open(pa, 'a').close();           msg = 'Đã tạo tệp tin: %s' % a
        elif op == 'rename' or op == 'move':
            shutil.move(pa, safe_path(b));   msg = 'Đã %s: %s → %s' % (
                'đổi tên' if op == 'rename' else 'di chuyển', a, b)
        elif op == 'copy':
            dst = safe_path(b)
            if os.path.isdir(pa):
                shutil.copytree(pa, dst)
            else:
                shutil.copy2(pa, dst)
            msg = 'Đã sao chép: %s → %s' % (a, b)
        elif op == 'delete':
            if os.path.isdir(pa):
                shutil.rmtree(pa)
            else:
                os.remove(pa)
            msg = 'Đã xóa: %s' % a
        elif op == 'chmod':
            os.chmod(pa, int(b, 8));         msg = 'Đã cấp quyền %s cho %s' % (b, a)
        elif op == 'compress':
            ok, out = run(['tar', '-zcvf', pa + '.tar.gz', '-C',
                           os.path.dirname(pa), os.path.basename(pa)])
            return J(ok, out if not ok else 'Đã nén: %s.tar.gz' % a)
        else:
            return J(False, 'Thao tác không hợp lệ.')
        return J(True, msg)
    except Exception as e:                              # noqa
        return J(False, 'Lỗi: %s' % e)


# ---- Lap lich crontab ----
@app.route('/api/cron/list', methods=['POST'])
def api_cron_list():
    ok, out = run(['crontab', '-l'])
    if not ok and 'no crontab' in out.lower():
        return J(True, '(Chưa có tác vụ nào được lập lịch)')
    return J(True, out)


@app.route('/api/cron/add', methods=['POST'])
def api_cron_add():
    line = request.form.get('line', '').strip()
    if not line:
        return J(False, 'Vui lòng nhập nội dung lịch.')
    cur = subprocess.run(['crontab', '-l'], capture_output=True, text=True).stdout
    new = (cur + line + '\n')
    p = subprocess.run(['crontab', '-'], input=new, text=True,
                       capture_output=True)
    return J(p.returncode == 0, p.stderr.strip() or 'Đã thêm tác vụ: %s' % line)


@app.route('/api/cron/delete', methods=['POST'])
def api_cron_delete():
    n = request.form.get('line_no', '')
    cur = subprocess.run(['crontab', '-l'], capture_output=True, text=True).stdout
    lines = [l for l in cur.splitlines() if l.strip()]
    try:
        idx = int(n) - 1
        if idx < 0 or idx >= len(lines):
            return J(False, 'Số thứ tự không hợp lệ.')
        removed = lines.pop(idx)
    except ValueError:
        return J(False, 'Số thứ tự không hợp lệ.')
    new = ('\n'.join(lines) + '\n') if lines else ''
    subprocess.run(['crontab', '-'], input=new, text=True)
    return J(True, 'Đã xóa tác vụ: %s' % removed)


@app.route('/api/cron/clear', methods=['POST'])
def api_cron_clear():
    ok, out = run(['crontab', '-r'])
    return J(True, 'Đã xóa toàn bộ tác vụ.')


# ---- Thoi gian he thong ----
@app.route('/api/time/get', methods=['POST'])
def api_time_get():
    ok, out = run(['date', '+%Y-%m-%d %H:%M:%S | %Z %z'])
    return J(ok, 'Thời gian hệ thống: ' + out)


@app.route('/api/time/set', methods=['POST'])
def api_time_set():
    kind = request.form.get('kind')
    val = request.form.get('value', '')
    if kind == 'time':
        ok, out = run(['sudo', 'date', '+%T', '-s', val])
    elif kind == 'date':
        ok, out = run(['sudo', 'date', '+%F', '-s', val])
    elif kind == 'tz':
        ok, out = run(['sudo', 'timedatectl', 'set-timezone', val])
    elif kind == 'ntp':
        run(['sudo', 'apt-get', 'install', '-y', 'ntpdate'])
        ok, out = run(['sudo', 'ntpdate', 'pool.ntp.org'])
    else:
        return J(False, 'Thao tác không hợp lệ.')
    return J(ok, out)


# ---- Goi phan mem ----
@app.route('/api/pkg/op', methods=['POST'])
def api_pkg_op():
    op = request.form.get('op')
    name = request.form.get('name', '').strip()
    if op == 'check':
        ok, out = run(['dpkg', '-s', name])
        return J(True, ('Gói "%s" ĐÃ được cài đặt.' % name) if ok
                 else ('Gói "%s" CHƯA được cài đặt.' % name))
    elif op == 'install':
        run(['sudo', 'apt-get', 'update'])
        ok, out = run(['sudo', 'apt-get', 'install', '-y', name], timeout=300)
    elif op == 'remove':
        ok, out = run(['sudo', 'apt-get', 'remove', '-y', name], timeout=300)
    elif op == 'update':
        ok, out = run(['sudo', 'apt-get', 'update'], timeout=300)
    elif op == 'search':
        ok, out = run(['apt-cache', 'search', name])
    else:
        return J(False, 'Thao tác không hợp lệ.')
    return J(ok, out)


# ================================================================ API PHAN 2
# ---- Tien trinh ----
@app.route('/api/proc/list', methods=['POST'])
def api_proc_list():
    p = subprocess.run(['ps', '-e', '-o', 'pid,ppid,stat,comm', '--sort=pid'],
                       capture_output=True, text=True)
    rows = []
    for line in p.stdout.splitlines()[1:]:
        parts = line.split(None, 3)
        if len(parts) == 4:
            rows.append({'pid': parts[0], 'ppid': parts[1],
                         'stat': parts[2], 'cmd': parts[3]})
    return jsonify(ok=True, rows=rows)


@app.route('/api/proc/run', methods=['POST'])
def api_proc_run():
    cmd = request.form.get('cmd', '').strip()
    if not cmd:
        return J(False, 'Vui lòng nhập lệnh.')
    ok, out = run(['/bin/sh', '-c', cmd], timeout=20)
    return J(ok, out)


@app.route('/api/proc/signal', methods=['POST'])
def api_proc_signal():
    pid = request.form.get('pid', '')
    sig = request.form.get('sig', '15')
    ok, out = run(['kill', '-' + sig, pid])
    return J(ok, out if not ok else 'Đã gửi tín hiệu %s tới tiến trình %s.' % (sig, pid))


# ---- Mang ----
@app.route('/api/net/info', methods=['POST'])
def api_net_info():
    p = subprocess.run(['ip', '-o', 'addr', 'show'], capture_output=True, text=True)
    rows = []
    for line in p.stdout.splitlines():
        f = line.split()
        if len(f) >= 4 and f[2] in ('inet', 'inet6'):
            rows.append({'iface': f[1],
                         'type': 'IPv4' if f[2] == 'inet' else 'IPv6',
                         'addr': f[3]})
    if not rows:
        ok, out = run(['hostname', '-I'])
        return J(ok, out)
    return jsonify(ok=True, rows=rows)


# ---- Socket TCP demo ----
def echo_server():
    """May chu TCP chay nen, phan hoi lai client (Phan 2 - server.c)."""
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        srv.bind(('127.0.0.1', SOCKET_PORT))
        srv.listen(5)
    except OSError:
        return
    while True:
        try:
            conn, _ = srv.accept()
            data = conn.recv(1024)
            if data:
                msg = data.decode('utf-8', 'replace').strip()
                conn.sendall(('[Server] Đã nhận: "%s" (%d ký tự)' %
                              (msg, len(msg))).encode('utf-8'))
            conn.close()
        except Exception:                               # noqa
            pass


@app.route('/api/socket/send', methods=['POST'])
def api_socket_send():
    msg = request.form.get('msg', '')
    try:
        c = socket.create_connection(('127.0.0.1', SOCKET_PORT), timeout=5)
        c.sendall(msg.encode('utf-8'))
        reply = c.recv(1024).decode('utf-8', 'replace')
        c.close()
        return J(True, 'Client gửi : %s\nServer trả: %s' % (msg, reply))
    except Exception as e:                              # noqa
        return J(False, 'Không kết nối được tới server: %s' % e)


# ================================================================ API PHAN 3
@app.route('/api/kernel/<action>', methods=['POST'])
def api_kernel(action):
    if action == 'build':
        ok, out = run(['make'], cwd=KERNEL_DIR, timeout=180)
    elif action == 'load':
        n = request.form.get('n', '5')
        ten = request.form.get('ten', 'Linux')
        ok, out = run(['sudo', 'insmod', 'hello_module.ko',
                       'n=%s' % n, 'ten=%s' % ten], cwd=KERNEL_DIR)
        if ok:
            out = 'Đã nạp module. Xem nhật ký nhân ở mục bên dưới.'
    elif action == 'unload':
        ok, out = run(['sudo', 'rmmod', 'hello_module'], cwd=KERNEL_DIR)
        if ok:
            out = 'Đã gỡ module hello_module.'
    elif action == 'status':
        ok, out = run(['lsmod'], cwd=KERNEL_DIR)
        lines = [l for l in out.splitlines() if l.startswith('hello_module')]
        return J(True, lines[0] if lines else 'Module hello_module hiện CHƯA được nạp.')
    elif action == 'dmesg':
        ok, out = run(['sh', '-c', 'dmesg | grep hello_module | tail -n 20'])
        return J(True, out or '(Chưa có nhật ký nào của module)')
    elif action == 'clean':
        ok, out = run(['make', 'clean'], cwd=KERNEL_DIR, timeout=60)
    else:
        return J(False, 'Thao tác không hợp lệ.')
    return J(ok, out)


if __name__ == '__main__':
    threading.Thread(target=echo_server, daemon=True).start()
    app.run(host='0.0.0.0', port=5000, debug=False)
