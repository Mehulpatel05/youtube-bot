import os
import json
import asyncio
import threading
from http.server import HTTPServer, BaseHTTPRequestHandler
import yt_dlp
from telegram import Update, InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import Application, CommandHandler, CallbackQueryHandler, MessageHandler, filters, ContextTypes

BOT_TOKEN = os.environ.get("BOT_TOKEN", "8345550172:AAFI4kVkebbzcRUQGVsRktz6FSZElJdWzXU")
CHANNEL_URL = "https://www.youtube.com/@Sandyz03/shorts"
DOWNLOAD_DIR = "downloads"
PENDING_FILE = "pending_videos.json"
DOWNLOADED_FILE = "downloaded.json"

os.makedirs(DOWNLOAD_DIR, exist_ok=True)


def load_json(path):
    if os.path.exists(path):
        with open(path) as f:
            return json.load(f)
    return {}


def save_json(path, data):
    with open(path, "w") as f:
        json.dump(data, f)


def fetch_shorts_list(count):
    ydl_opts = {
        "quiet": True,
        "extract_flat": True,
        "playlist_end": count,
        "ignoreerrors": True,
        "socket_timeout": 30,
        "extractor_args": {"youtubetab": {"skip": ["authcheck"]}},
    }
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        info = ydl.extract_info(CHANNEL_URL, download=False)
        if not info:
            return []
        entries = info.get("entries", [])
        if entries and isinstance(entries[0], dict) and "entries" in entries[0]:
            entries = entries[0]["entries"]
        results = []
        for e in entries[:count]:
            if not e or not e.get("id"):
                continue
            results.append({
                "id": e["id"],
                "title": e.get("title", e["id"]),
                "url": f"https://www.youtube.com/shorts/{e['id']}",
                "views": e.get("view_count") or 0,
            })
        return results


def download_video(url: str):
    ydl_opts = {
        "outtmpl": f"{DOWNLOAD_DIR}/%(id)s.%(ext)s",
        "format": "mp4/best[ext=mp4]/best",
        "quiet": True,
    }
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        info = ydl.extract_info(url, download=True)
        return ydl.prepare_filename(info), info.get("description") or "N/A", info.get("uploader") or "N/A"


async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text("Namaste! /fetch likhein YouTube Shorts fetch karne ke liye.")


async def fetch(update: Update, context: ContextTypes.DEFAULT_TYPE):
    context.user_data["waiting_for_count"] = True
    await update.message.reply_text("Kitne Shorts download karne hain? (number likhein, jaise 5)")


async def handle_count(update: Update, context: ContextTypes.DEFAULT_TYPE):
    if not context.user_data.get("waiting_for_count"):
        return

    text = update.message.text.strip()
    if not text.isdigit() or int(text) < 1:
        await update.message.reply_text("Sirf number likhein, jaise 5")
        return

    count = int(text)
    context.user_data["waiting_for_count"] = False

    await update.message.reply_text(f"⏳ YouTube se {count} Shorts fetch ho rahi hain, thoda wait karein...")

    try:
        videos = await asyncio.get_event_loop().run_in_executor(None, fetch_shorts_list, count)
    except Exception as e:
        await update.message.reply_text(f"❌ Error: {e}")
        return

    if not videos:
        await update.message.reply_text("Koi shorts nahi mili.")
        return

    downloaded = load_json(DOWNLOADED_FILE)
    pending = load_json(PENDING_FILE)

    new_count = 0
    for v in videos:
        if v["id"] not in downloaded:
            pending[v["id"]] = v
            new_count += 1

    save_json(PENDING_FILE, pending)

    if new_count == 0:
        await update.message.reply_text("✅ Saari videos pehle se download ho chuki hain.")
        return

    await update.message.reply_text(f"✅ {new_count} nayi Shorts mili:")

    for v in videos:
        if v["id"] in downloaded:
            await update.message.reply_text(f"⏭️ Already downloaded: {v['title']}")
            continue
        keyboard = [[InlineKeyboardButton("📤 Send to Telegram", callback_data=f"send_{v['id']}")]]
        info_text = f"🎬 {v['title']}\n👁 Views: {v.get('views', 0):,}\n🔗 {v['url']}"
        await update.message.reply_text(info_text, reply_markup=InlineKeyboardMarkup(keyboard))


async def send_video(update: Update, context: ContextTypes.DEFAULT_TYPE):
    query = update.callback_query
    await query.answer()

    video_id = query.data.replace("send_", "")
    pending = load_json(PENDING_FILE)
    downloaded = load_json(DOWNLOADED_FILE)

    video = pending.get(video_id)
    if not video:
        await query.edit_message_text("Video nahi mili.")
        return

    await query.edit_message_text(f"⬇️ Download ho rahi hai: {video['title']}")

    try:
        filepath, description, channel = await asyncio.get_event_loop().run_in_executor(None, download_video, video["url"])
        desc = description[:800] + "..." if len(description) > 800 else description
        caption = f"🎬 {video['title']}\n📺 {channel}\n👁 {video.get('views', 0):,} views\n\n📝 {desc}"
        with open(filepath, "rb") as vf:
            await query.message.reply_video(video=vf, caption=caption)
        os.remove(filepath)

        downloaded[video_id] = video["title"]
        save_json(DOWNLOADED_FILE, downloaded)
        pending.pop(video_id, None)
        save_json(PENDING_FILE, pending)

        await query.edit_message_text(f"✅ Bhej diya: {video['title']}")
    except Exception as e:
        await query.edit_message_text(f"❌ Error: {e}")


class HealthHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"OK")

    def log_message(self, format, *args):
        pass


def main():
    port = int(os.environ.get("PORT", 8080))
    threading.Thread(target=lambda: HTTPServer(("", port), HealthHandler).serve_forever(), daemon=True).start()
    app = Application.builder().token(BOT_TOKEN).build()
    app.add_handler(CommandHandler("start", start))
    app.add_handler(CommandHandler("fetch", fetch))
    app.add_handler(CallbackQueryHandler(send_video, pattern="^send_"))
    app.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, handle_count))
    print("Bot chal raha hai...")
    app.run_polling()


if __name__ == "__main__":
    main()
