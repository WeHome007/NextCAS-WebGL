import { defineConfig } from 'vite'
import { svelte } from '@sveltejs/vite-plugin-svelte'

const ModuleToDefer = () => {
  return {
    name: "ModuleToDefer",
    transformIndexHtml(html) {
      return html.replace(`type="module" crossorigin`, "defer");
    }
  }
};

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [svelte(), ModuleToDefer()],
  base: './',
})
