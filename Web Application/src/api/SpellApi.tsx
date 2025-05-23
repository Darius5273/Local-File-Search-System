import axios from 'axios';
import { SpellRequest } from '../interfaces/SpellRequest';

const axiosInstance = axios.create({
  baseURL: 'http://localhost:8080',
  timeout: 10000,
  headers: { 'Content-Type': 'application/json' },
});

export async function setSpellStrategy(strategy: string): Promise<void> {
  try {
    await axiosInstance.post('/spell', { spellStrategy: strategy } as SpellRequest);
  } catch (error) {
    throw new Error('Failed to update spell strategy');
  }
}

export const getSpellStrategy = async (): Promise<string> => {
  try {
    const response = await axiosInstance.get('/spell-strategy');
    return response.data.strategy;
  } catch (error) {
    throw new Error('Failed to fetch current spell strategy');
  }
};