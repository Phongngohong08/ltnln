/* Tien ich goi API va hien thi ket qua cho cong quan tri Linux */
function api(url, data) {
  const body = new URLSearchParams(data || {});
  return fetch(url, {
    method: 'POST',
    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
    body: body
  }).then(r => r.json());
}

/* Hien ket qua dang console (khoi den) tai phan tu #id */
function showResult(id, res) {
  const el = document.getElementById(id);
  if (!el) return;
  el.classList.remove('ok', 'err');
  el.classList.add(res.ok ? 'ok' : 'err');
  el.textContent = (res.ok ? '[THÀNH CÔNG]\n' : '[THẤT BẠI]\n') + (res.output || '');
}

/* Hien thong bao ngan (flash) */
function flash(id, ok, msg) {
  const el = document.getElementById(id);
  if (!el) return;
  el.className = 'flash ' + (ok ? 'ok' : 'err');
  el.textContent = msg;
}

/* Lay gia tri input theo id */
function val(id) {
  const el = document.getElementById(id);
  return el ? el.value.trim() : '';
}

/* Xac nhan truoc khi thuc hien thao tac nguy hiem */
function confirmDo(msg, fn) {
  if (window.confirm(msg)) fn();
}
