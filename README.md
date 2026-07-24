# Sanjeevani AI - Rural Healthcare Management System

An AI-powered, offline-resilient, SaaS-grade rural healthcare management web application designed for Primary Health Centers (PHCs) and community clinics.

## Features & Modules

- 🎨 **Modern SaaS UI/UX**: Clean, responsive, glassmorphic dark/light aesthetic built with Tailwind CSS, Chart.js, and Lucide icons.
- 🌐 **Multilingual UI**: Live switching between English, Hindi (`हिन्दी`), and Tamil (`தமிழ்`).
- 🤖 **AI Clinical Decision Support**: LLM-powered (Claude/OpenAI with automatic fallback heuristic engine) diagnostic differentials, lab test recommendations, and care plans with mandatory physician disclaimers.
- 📋 **Automated Triage Queue**: Rule-based priority assignment (`Emergency`, `High`, `Normal`) based on symptoms and vitals.
- 💊 **Medicine Inventory & Smart Expiry Alerts**: Pre-seeded with ~150 WHO/Jan Aushadhi generic medicines, batch stock tracking, cold chain flags, and moving-average reorder forecaster.
- ⚠️ **Prescription Drug Interaction Checker**: Instant safety warnings when prescribing combinations with known contraindications.
- 🎙️ **Voice Assistant Intake**: Web Speech API integration for recording patient symptoms in local languages.
- 📱 **Digital Patient QR Health Card**: Generate downloadable QR cards for quick EHR retrieval.
- 📊 **Admin Analytics & Outbreak Heatmap**: Footfall trend charts, top diagnostic categories, and village-wise symptom cluster outbreak monitoring.
- 📲 **AI Reminders**: Multilingual SMS/WhatsApp follow-up message generator.

---

## Quick Start Instructions

### 1. Requirements
- Python 3.10+
- `pip`

### 2. Setup Virtual Environment & Install Dependencies
```bash
python -m venv venv
# On Windows PowerShell:
.\venv\Scripts\Activate.ps1
# On Linux/macOS:
source venv/bin/activate

pip install -r requirements.txt
```

### 3. Environment Variables (Optional for LLM API Key)
Copy `.env.example` to `.env`:
```bash
cp .env.example .env
```
Edit `.env` if you wish to set your `AI_API_KEY`. If left blank, Sanjeevani AI automatically runs its built-in clinical heuristic engine so all features work out-of-the-box!

### 4. Run the Application
```bash
uvicorn main:app --reload --port 8000
```

Open your browser and navigate to:
**`http://localhost:8000`**

---

## Pre-seeded Logins & Demo Accounts

### Staff Accounts (No password required, use Staff ID):
- **Doctor**: `DOC001` (Dr. Ananya Sharma)
- **Nurse/Triage**: `NUR001` (Nurse Rajesh Kumar)
- **Pharmacist**: `PHM001` (Pharmacist Suresh Verma)
- **Admin**: `ADM001` (Admin Priya Patel)

### Patient Account (No password required, use Phone Number):
- **Phone**: `9876543210` (Ramesh Kumar)
