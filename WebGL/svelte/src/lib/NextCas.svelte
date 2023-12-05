<script lang="ts">
  import { onMount, onDestroy } from "svelte";
  import NextCas from "@nextcas/sdk";
  import type { ReplayEvent } from "@nextcas/sdk";
  let container: HTMLDivElement;

  let cas: NextCas;

  let progress: number = 0;

  let inited = false;
  let inputValue = "";

  async function getToken() {
   return {data:''}
  }

  let chatHistory: { source: "nexthuman" | "guest"; content: string }[] = [
    {
      source: "nexthuman",
      content: "你好，请问有什么可以帮您",
    },
  ];

  onMount(async () => {
    const { data: token } = await getToken();
    cas = new NextCas(container, {
      avatarId: "avatar_257",
      actorId: "641811add41a3f2f91247af5",
      token,
      templateName: "introduce",
      // src:"http://127.0.0.1:5173/demo"
    });

    cas.on("initProgress", (cent) => {
      progress = cent;
    });

    cas.on("ready", () => {
      inited = true;
      setTimeout(() => {
        cas.speak("你好，请问有什么可以帮您");
      });
    });
  });

  onDestroy(() => {
    if (cas) {
      cas.destroy();
    }
  });

  const reload = async () => {
    if (cas) {
      inited = false;
      progress = 0;
      const token = await getToken();
      cas.reload(token.data);
    }
  };

  const setTemplate = (templateName: "base" | "introduce") => {
    inited = false;
    cas?.setTemplate(templateName);
  };

  const speak = () => {
    chatHistory.push({
      source: "nexthuman",
      content: inputValue,
    });
    chatHistory = chatHistory;
    cas?.speak(inputValue);
  };

  const ask = async () => {
    chatHistory.push({
      source: "guest",
      content: inputValue,
    });
    const askId = await cas.ask(inputValue);

    const index = chatHistory.length;
    function reply(data: ReplayEvent) {
      if (data.id === askId) {
        if (!chatHistory[index]) {
          chatHistory.push({
            source: "nexthuman",
            content: data.data.content,
          });
        } else {
          chatHistory[index].content += data.data.content;
        }
        chatHistory = chatHistory;

        if (data.data.last) {
          // 结束了
          cas.off("reply", reply);
        }
      }
    }
    cas.on("reply", reply);
  };

  const speackStream = () => {
    const stream = cas?.createSpeackStream();
    stream.next("你好");
    setTimeout(() => {
      stream.next("我是小唯");
      stream.last("很高兴见到你");
    }, 1000);
  };

  const stopAct = () => {
    cas?.call("stopAct");
    // console.log(cas)
  };


 async function setAvatar(avatarId:string,actorId:string){
   await cas?.setAvatar(avatarId);
   await cas?.setActor(actorId);
   
  }
</script>

<div style="display: flex;">
  <div
    style="width:375px;height:800px;border:red 1px solid;flex-shrink:0"
    bind:this={container}
  />

  <div class="apis">
    <div class="api-box">
      <div class="api-title">初始化状态：</div>
      {inited ? "初始化完成" : "正在加载" + progress + "%"}
    </div>
    <div class="api-box">
      <div class="api-title">角色切换：</div>
      <button
        on:click={() => {
          setAvatar("avatar_257","641811add41a3f2f91247af5");
        }}>女</button
      >
      <button
        on:click={() => {
          setAvatar("avatar_1078","actor_100230");
        }}>男</button
      >
    </div>
    <div class="api-box">
      <div class="api-title">模板切换：</div>
      <button
        on:click={() => {
          setTemplate("introduce");
        }}>setTemplate("introduce")</button
      >
      <button
        on:click={() => {
          setTemplate("base");
        }}>setTemplate("base")</button
      >
    </div>
    <div class="api-box">
      <div class="api-title">重新加载：</div>
      <button on:click={reload}>reload</button>
    </div>

    <div class="api-box">
      <div class="api-title">视角切换：</div>
      <button
        on:click={() => {
          cas?.changeCamera(
            { x: 0, y: 1.65, z: 0.58 },
            { x: 0, y: 1.61, z: -0.45 },
            500
          );
        }}>脸部视角</button
      >

      <button
        on:click={() => {
          cas?.changeCamera(
            { x: 0, y: 1.65, z: 2.42 },
            { x: 0, y: 1.34, z: 1.5 },
            500
          );
        }}>全身视角</button
      >
    </div>

    <div class="api-box">
      <div class="api-title">装扮切换：</div>
      <button
        on:click={async () => {
          await cas?.addBundle("hair_1001");
          console.log("switch success!!");
        }}>头发</button
      >
    </div>

    <div class="api-box">
      <div class="api-title">AI对话：</div>
      <textarea bind:value={inputValue} />
      <button on:click={ask}>ask</button>
      <button on:click={speak}>speak</button>
      <button on:click={stopAct}>打断演讲</button>
      <button on:click={speackStream}>流式演讲</button>
    </div>

    {#if !!inited}
      <div class="chat-history">
        {#each chatHistory as chat}
          <div class="chat-item">
            <div class="chat-item-avatar">
              {chat.source}：
            </div>
            <div class="chat-item-content">{chat.content}</div>
          </div>
        {/each}
      </div>
    {/if}
  </div>
</div>

<style>
  .apis {
    padding: 20px;
    max-width: 500px;
  }
  .api-box {
    display: flex;
    margin-top: 6px;
  }
  .api-title {
    font-size: 18px;
    font-weight: bold;
  }

  .chat-history {
    margin-top: 24px;
    max-height: 400px;
    overflow-y: overlay;
  }
  .chat-item {
    display: flex;
  }
  .chat-item-content {
    text-align: center;
  }
  .chat-item-avatar {
    font-weight: bold;
    font-size: 18px;
  }
  button {
    margin: 0 12px;
  }
</style>
