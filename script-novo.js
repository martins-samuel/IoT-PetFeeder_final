import { initializeApp } from "https://www.gstatic.com/firebasejs/10.12.0/firebase-app.js";
import { getDatabase, ref, set, onValue } from "https://www.gstatic.com/firebasejs/10.12.0/firebase-database.js";

const firebaseConfig = {
  apiKey: "",
  authDomain: "",
  databaseURL: "",
  projectId: "pet-feeder-iotnodemcu",
  storageBucket: "",
  messagingSenderId: "",
  appId: ""
};

const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

document.addEventListener('DOMContentLoaded', () => {
  const liberarBtn = document.getElementById('liberar-btn');
  const agendarForm = document.getElementById('agendar-form');
  const nivelInfo = document.getElementById('nivel-info');
  const historicoLista = document.getElementById('historico-lista');

  liberarBtn.addEventListener('click', async () => {
    try {
      await set(ref(db, 'alimentar'), true);
      alert('Comando enviado para o alimentador!');
    } catch (error) {
      console.error(error);
      alert('Erro ao enviar comando.');
    }
  });

  agendarForm.addEventListener('submit', async (e) => {
    e.preventDefault();
    const hora1 = document.getElementById('hora1').value;
    const hora2 = document.getElementById('hora2').value;
    try {
      await set(ref(db, 'agendamentos'), { hora1, hora2 });
      alert('Horários salvos com sucesso!');
    } catch (error) {
      console.error(error);
      alert('Erro ao salvar horários');
    }
  });

  // Atualizar nível de ração com emojis e 4 níveis
  onValue(ref(db, 'nivel_racao_cm'), (snapshot) => {
    const nivel = snapshot.val();
    if (nivel !== null) {
      let status = '';
      let emoji = '';

      if (nivel > 12) {
        status = 'Cheio';
        emoji = '🟢';
      } else if (nivel > 8) {
        status = 'Médio';
        emoji = '🟡';
      } else if (nivel > 4) {
        status = 'Baixo';
        emoji = '🟠';
      } else {
        status = 'Crítico';
        emoji = '🔴';
      }

      nivelInfo.textContent = `${emoji} Nível atual: ${status} (${nivel.toFixed(2)} cm)`;
    } else {
      nivelInfo.textContent = "Nível não disponível";
    }
  });

  // Atualizar histórico
  onValue(ref(db, 'historico'), (snapshot) => {
    historicoLista.innerHTML = '';
    const dados = snapshot.val();
    if (dados) {
      const chaves = Object.keys(dados).sort().reverse();
      chaves.forEach((key) => {
        const li = document.createElement('li');
        li.textContent = `${key.replace('_', ' ')} → ${dados[key]}`;
        historicoLista.appendChild(li);
      });
    }
  });
});
